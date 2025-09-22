#include "proc_reader.h"

int list_process_directories(void) {
    // TODO: Open the /proc directory using opendir()
       DIR *dir = opendir("/proc");
    // TODO: Check if opendir() failed and print error message
       if (dir == NULL) {
        perror("opendir failed");
        return -1;
    }

    // TODO: Declare a struct dirent pointer for directory entries
       struct dirent *entry;

    // TODO: Initialize process counter to 0
       int process_count = 0;     
    printf("Process directories in /proc:\n");
    printf("%-8s %-20s\n", "PID", "Type");
    printf("%-8s %-20s\n", "---", "----");

    // TODO: Read directory entries using readdir() in a loop
       while ((entry = readdir(dir)) != NULL) {

    // TODO: For each entry, check if the name is a number using is_number()
       if (is_number(entry->d_name)) {
    // TODO: If it's a number, print it as a PID and increment counter
       printf("%-8s %-20s\n", entry->d_name, "process");
            process_count++;
        }
    }

    // TODO: Close the directory using closedir()
       if (closedir(dir) == -1) {
        perror("closedir failed");
        return -1;
    }
    // TODO: Check if closedir() failed
       
           
    // TODO: Print the total count of process directories found
       printf("Found %d process directories\n", process_count);

    return 0; // Replace with proper error handling
}

int read_process_info(const char* pid) {
    char filepath[256];

    // TODO: Create the path to /proc/[pid]/status using snprintf()
       snprintf(filepath, sizeof(filepath), "/proc/%s/status", pid);
   
    printf("\n--- Process Information for PID %s ---\n", pid);

    // TODO: Call read_file_with_syscalls() to read the status file
       if (read_file_with_syscalls(filepath) == -1) {
        fprintf(stderr, "Failed to read %s\n", filepath);
        return -1;
    }
    // TODO: Check if the function succeeded
       
    // TODO: Create the path to /proc/[pid]/cmdline using snprintf()
       snprintf(filepath, sizeof(filepath), "/proc/%s/cmdline", pid);

    printf("\n--- Command Line ---\n");

    // TODO: Call read_file_with_syscalls() to read the cmdline file
       if (read_file_with_syscalls(filepath) == -1) {
        fprintf(stderr, "Failed to read %s\n", filepath);
        return -1;
    }
    // TODO: Check if the function succeeded

    printf("\n"); // Add extra newline for readability

    return 0; // Replace with proper error handling
}

int show_system_info(void) {
    int line_count = 0;
    const int MAX_LINES = 10;
    char line[256];
    printf("\n--- CPU Information (first %d lines) ---\n", MAX_LINES);

    // TODO: Open /proc/cpuinfo using fopen() with "r" mode
       FILE *cpu_file = fopen("/proc/cpuinfo", "r");

    // TODO: Check if fopen() failed
       if (!cpu_file) {
        perror("fopen /proc/cpuinfo");
        return -1;
    }
    // TODO: Declare a char array for reading lines       
    // TODO: Read lines using fgets() in a loop, limit to MAX_LINES
    // TODO: Print each line
        line_count = 0;
    while (fgets(line, sizeof(line), cpu_file) != NULL && line_count < MAX_LINES) {
        printf("%s", line);
        line_count++;
    }
    // TODO: Close the file using fclose()
       fclose(cpu_file);

    printf("\n--- Memory Information (first %d lines) ---\n", MAX_LINES);

    // TODO: Open /proc/meminfo using fopen() with "r" mode
       FILE *mem_file = fopen("/proc/meminfo", "r");
      
    
    // TODO: Check if fopen() failed
       if (!mem_file) {
        perror("fopen /proc/meminfo");
        return -1;
    }
    // TODO: Read lines using fgets() in a loop, limit to MAX_LINES       
    // TODO: Print each line
       line_count = 0;
    while (fgets(line, sizeof(line), mem_file) != NULL && line_count < MAX_LINES) {
        printf("%s", line);
        line_count++;
    }
    // TODO: Close the file using fclose()
       fclose(mem_file);

    return 0; // Replace with proper error handling
}

void compare_file_methods(void) {
    const char* test_file = "/proc/version";

    printf("Comparing file reading methods for: %s\n\n", test_file);

    printf("=== Method 1: Using System Calls ===\n");
    read_file_with_syscalls(test_file);

    printf("\n=== Method 2: Using Library Functions ===\n");
    read_file_with_library(test_file);

    printf("\nNOTE: Run this program with strace to see the difference!\n");
    printf("Example: strace -e trace=openat,read,write,close ./lab2\n");
}

int read_file_with_syscalls(const char* filename) {
    // TODO: Declare variables: file descriptor (int), buffer (char array), bytes_read (ssize_t)
        int fd;
        char buffer[1024];
        ssize_t bytes_read;
    // TODO: Open the file using open() with O_RDONLY flag
       fd = open(filename, O_RDONLY);
     
    // TODO: Check if open() failed (fd == -1) and return -1
       if (fd == -1) {
        perror("open failed");
        return -1;
    }

    // TODO: Read the file in a loop using read()
       while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {

    // TODO: Use sizeof(buffer) - 1 for buffer size to leave space for null terminator
    // TODO: Check if read() returns > 0 (data was read)

    // TODO: Null-terminate the buffer after each read
        buffer[bytes_read] = '\0';
    // TODO: Print each chunk of data read
       printf("%s", buffer);
    }
    // TODO: Handle read() errors (return value -1)
    // TODO: If read() fails, close the file and return -1
       if (bytes_read == -1) {
        perror("read failed");
        close(fd);
        return -1;
    }
    // TODO: Close the file using close()
       if (close(fd) == -1) {
        // TODO: Check if close() failed
        perror("close failed");
        return -1;
    }

    // TODO: Check if close() failed

    return 0; // Replace with proper error handling
}

int read_file_with_library(const char* filename) {
    // TODO: Declare variables: FILE pointer, buffer (char array)
       FILE *file;
       char line[256];        
    // TODO: Open the file using fopen() with "r" mode
        file = fopen(filename, "r");
    // TODO: Check if fopen() failed and return -1
       if (!file) {
        perror("fopen failed");
        return -1;
    }

    // TODO: Read the file using fgets() in a loop
    // TODO: Continue until fgets() returns NULL
    // TODO: Print each line read
        while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }


    // TODO: Close the file using fclose()
        if (fclose(file) != 0) {

    // TODO: Check if fclose() failed
         perror("fclose failed");
        return -1;
    }
    return 0; // Replace with proper error handling
}

int is_number(const char* str) {
    // TODO: Handle empty strings - check if str is NULL or empty
        if (str == NULL || *str == '\0') {

    // TODO: Return 0 for empty strings
       return 0;
    }
    // TODO: Check if the string contains only digits     
    // TODO: Loop through each character using a while loop
    // TODO: Use isdigit() function to check each character
       const char *ptr = str;
    while (*ptr) {

    // TODO: If any character is not a digit, return 0
       if (!isdigit(*ptr)) {
            return 0;
        }
        ptr++;
    }       
    // TODO: Return 1 if all characters are digits
       
    return 1; // Replace with actual implementation
}
