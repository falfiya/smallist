#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>

C_ASSERT(sizeof(size_t) == 8);

#define PATH_MAX_LEN (4 * MAX_PATH)
#define DWORD_MAX 4294967295
#define DWORD_MAX_LEN 10

// returns the length
DWORD dwToChars(DWORD dw, char *out) {
   DWORD length;
   if (dw > 1000000000) {
      length = 10;
   } else if (dw >= 100000000) {
      length = 9;
   } else if (dw >= 10000000) {
      length = 8;
   } else if (dw >= 1000000) {
      length = 7;
   } else if (dw >= 100000) {
      length = 6;
   } else if (dw >= 10000) {
      length = 5;
   } else if (dw >= 1000) {
      length = 4;
   } else if (dw >= 100) {
      length = 3;
   } else if (dw >= 10) {
      length = 2;
   } else {
      *out = '0' + dw;
      return 1;
   }

   out += length;
   while (dw > 0) {
      out--;
      *out = '0' + dw % 10;
      dw /= 10;
   }

   return length;
}

void start(void) {
   HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
   HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if (hStdout == INVALID_HANDLE_VALUE || snap == INVALID_HANDLE_VALUE) {
      ExitProcess(1);
      __builtin_unreachable();
   }

   PROCESSENTRY32W entry;
   entry.dwSize = sizeof(PROCESSENTRY32W);

   if (!Process32FirstW(snap, &entry)) {
      // let's let Windows close it for us
      //CloseHandle(snap);
      ExitProcess(1);
      __builtin_unreachable();
   }

   SYSTEM_INFO sys_info;
   GetSystemInfo(&sys_info);
   // let's just allocate 20 pages of memory to be on the safe side.
   // that means that once that memory is used, it's flushed and reused
   // this is about 40 KB which isn't actually that much if you think about
   // it.
   DWORD iobuf_sz = sys_info.dwPageSize * 20;
   char *const iobuf = VirtualAlloc(
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

      if (offset + max_size > iobuf_sz) {
         if (!WriteFile(hStdout, iobuf, offset, NULL, NULL)) {
            ExitProcess(1);
            __builtin_unreachable();
         }
         offset = 0;
         WriteFile(hStdout, "flush\n", 6, NULL, NULL);
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
   } while (Process32NextW(snap, &entry));

   if (!WriteFile(hStdout, iobuf, offset, NULL, NULL)) {
      ExitProcess(1);
      __builtin_unreachable();
   }
   // let's let Windows close it for us
   //CloseHandle(snap);
   ExitProcess(0);
   __builtin_unreachable();
}
