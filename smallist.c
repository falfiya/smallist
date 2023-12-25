#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>

#define PATH_MAX_LEN (4 * MAX_PATH)
#define DWORD_MAX 4294967295
#define DWORD_MAX_LEN 10

// comment this out to enable closing handles (pay 28 bytes)
// this does nothing because all handles are closed when the process exists
#define CloseHandle(_)

#if 1
// pay a penalty of 16 bytes for these
#define UNLIKELY(prop) __builtin_expect(prop, 0)
#define LIKELY(prop) __builtin_expect(prop, 1)
#else
#define UNLIKELY(prop) prop
#define LIKELY(prop) prop
#endif

// returns the length
DWORD dwToChars(DWORD dw, char *out) {
   DWORD length = 1;
   DWORD mul_10 = 10;

   if (UNLIKELY(dw == 0)) {
      *out = '0';
      return 1;
   }

   while (LIKELY(dw >= mul_10)) {
      length++;
      mul_10 *= 10;
   }

   out += length;
   while (LIKELY(dw > 0)) {
      out--;
      *out = '0' + dw % 10;
      dw /= 10;
   }

   return length;
}

void start(void) {
   HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
   HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if (UNLIKELY(hStdout == INVALID_HANDLE_VALUE || snap == INVALID_HANDLE_VALUE)) {
      ExitProcess(1);
      __builtin_unreachable();
   }

   PROCESSENTRY32W entry;
   entry.dwSize = sizeof(PROCESSENTRY32W);

   if (UNLIKELY(!Process32FirstW(snap, &entry))) {
      CloseHandle(snap);
      ExitProcess(1);
      __builtin_unreachable();
   }

   // 16 pages of memory / 64 KiB
   #define iobuf_sz 0x10000
   char *const restrict iobuf = VirtualAlloc(
      NULL,
      iobuf_sz,
      MEM_COMMIT | MEM_RESERVE,
      PAGE_READWRITE
   );

   DWORD offset = 0;
   do {
      DWORD max_size = 0
         + DWORD_MAX_LEN + 1
         + DWORD_MAX_LEN + 1
         + PATH_MAX_LEN + 1;

      if (UNLIKELY(offset + max_size > iobuf_sz)) {
         if (UNLIKELY(!WriteFile(hStdout, iobuf, offset, NULL, NULL))) {
            CloseHandle(snap);
            ExitProcess(1);
            __builtin_unreachable();
         }
         offset = 0;
      }

      offset += dwToChars(entry.th32ProcessID, iobuf + offset);
      iobuf[offset] = '\t'; offset += 1;
      offset += dwToChars(entry.th32ParentProcessID, iobuf + offset);
      iobuf[offset] = '\t'; offset += 1;
      // codecvt
      offset += WideCharToMultiByte(
         CP_UTF8,
         0,
         entry.szExeFile,
         -1, // infer size because szExeFile is null terminated
         iobuf + offset,
         PATH_MAX_LEN, // size of output buffer
         NULL,
         NULL
      ) - 1; // minus one because of the null byte
      iobuf[offset] = '\n'; offset += 1;
   } while (LIKELY(Process32NextW(snap, &entry)));

   if (UNLIKELY(!WriteFile(hStdout, iobuf, offset, NULL, NULL))) {
      CloseHandle(snap);
      ExitProcess(1);
      __builtin_unreachable();
   }

   CloseHandle(snap);
   ExitProcess(0);
   __builtin_unreachable();
}
