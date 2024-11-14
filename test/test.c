#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

extern int main(int argc, char *argv[]);

static int tests_run = 0;
static int tests_failed = 0;

// == HELPERS ================================================

void create_test_file(const char *path, const char *content) {
    FILE *f = fopen(path, "w");
    if (f) {
        fputs(content, f);
        fclose(f);
    }
}

void clean_test_file(const char *path) {
    remove(path);
}

int redirect_output() {
    int fd_null = open("/dev/null", O_WRONLY);
    if (fd_null == -1) {
        perror("open");
        return 1;
    }

    if (dup2(fd_null, STDOUT_FILENO) == -1 || dup2(fd_null, STDERR_FILENO) == -1) {
        perror("dup2");
        close(fd_null);
        return 1;
    }

    close(fd_null);
    return 0;
}

void restore_output(int saved_stdout, int saved_stderr) {
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stdout);
    close(saved_stderr);
}

// == MACROS =================================================

#define RESET     "\033[0m"
#define RED       "\033[31m"
#define GREEN     "\033[32m"
#define BOLDWHITE "\033[1m\033[37m"

#define RUN_TEST(test) do {                              \
    printf(BOLDWHITE "running test: %s\n" RESET, #test); \
    tests_run++;                                         \
    test();                                              \
} while(0)

#define ASSERT(msg, cond) do {                                 \
    if (!(cond)) {                                             \
        tests_failed++;                                        \
        printf("assertion failed in %s: %s\n", __func__, msg); \
        return;                                                \
    }                                                          \
} while(0)

// == TESTS ==================================================

void test_no_args() {
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    redirect_output();

    char *argv[] = { "pangolin" };
    int result = main(1, argv);

    restore_output(saved_stdout, saved_stderr);

    ASSERT(RED "❌ should fail with no args" RESET, result == 1);
    printf(GREEN "✅ test_no_args() passed!\n" RESET);
}

void test_help_flag() {
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    redirect_output();

    char *argv[] = { "pangolin", "--help" };
    int result = main(2, argv);

    restore_output(saved_stdout, saved_stderr);

    ASSERT(RED "❌ help flag should exit successfully" RESET, result == 0);
    printf(GREEN "✅ test_help_flag() passed!\n" RESET);
}

void test_invalid_flag() {
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    redirect_output();

    char *argv[] = { "pangolin", "--invalid", "test.pang" };
    int result = main(3, argv);

    restore_output(saved_stdout, saved_stderr);

    ASSERT(RED "❌ should fail with invalid flag" RESET, result == 1);
    printf(GREEN "✅ test_invalid_flag() passed!\n" RESET);
}

void test_missing_output_file() {
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    redirect_output();

    char *argv[] = { "pangolin", "--output" };
    int result = main(2, argv);

    restore_output(saved_stdout, saved_stderr);

    ASSERT(RED "❌ should fail when output file is not specified" RESET, result == 1);
    printf(GREEN "✅ test_missing_output_file() passed!\n" RESET);
}

void test_multiple_input_files() {
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    redirect_output();

    char *argv[] = { "pangolin", "input1.pang", "input2.pang" };
    int result = main(3, argv);

    restore_output(saved_stdout, saved_stderr);

    ASSERT(RED "❌ should fail with multiple input files" RESET, result == 1);
    printf(GREEN "✅ test_multiple_input_files() passed!\n" RESET);
}

void test_valid_file_reading() {
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    redirect_output();

    const char* test_file = "test_input.pang";
    const char* test_content = "hello, pangolin\n";
    
    create_test_file(test_file, test_content);
    
    char *argv[] = { "pangolin", test_file };
    int result = main(2, argv);

    restore_output(saved_stdout, saved_stderr);

    ASSERT(RED "❌ should succeed with valid input file" RESET, result == 0);
    clean_test_file(test_file);
    printf(GREEN "✅ test_valid_file_reading() passed!\n" RESET);
}

void test_nonexistent_file() {
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    redirect_output();

    char *argv[] = { "pangolin", "nonexistent.pang" };
    int result = main(2, argv);

    restore_output(saved_stdout, saved_stderr);

    ASSERT(RED "❌ should fail with nonexistent file" RESET, result == 1);
    printf(GREEN "✅ test_nonexistent_file() passed!\n" RESET);
}

void test_debug_flag() {
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);
    redirect_output();

    const char* test_file = "debug_test.pang";
    create_test_file(test_file, "debug, pangolin!\n");
    
    char *argv[] = { "pangolin", "--debug", test_file };
    int result = main(3, argv);

    restore_output(saved_stdout, saved_stderr);

    ASSERT(RED "❌ should succeed with debug flag" RESET, result == 0);
    clean_test_file(test_file);
    printf(GREEN "✅ test_debug_flag() passed!\n" RESET);
}

// == RUN ====================================================

__attribute__((constructor))
void run_tests() {
    printf(BOLDWHITE "running tests...\n" RESET);

    RUN_TEST(test_no_args);
    RUN_TEST(test_help_flag);
    RUN_TEST(test_invalid_flag);
    RUN_TEST(test_missing_output_file);
    RUN_TEST(test_multiple_input_files);
    RUN_TEST(test_valid_file_reading);
    RUN_TEST(test_nonexistent_file);
    RUN_TEST(test_debug_flag);

    printf(BOLDWHITE "\ntest summary:\n");
    printf("  tests run: %d\n", tests_run);
    printf("  tests failed: %d\n" RESET, tests_failed);

    if (tests_failed > 0) {
        printf(RED "\n😭 some tests failed...\n" RESET);
        exit(1);
    } else {
        printf(GREEN "\n😀 all tests passed!\n" RESET);
        exit(0);
    }
}