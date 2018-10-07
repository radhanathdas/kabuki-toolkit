#pragma once
#if SEAM == SEAM_0_0_0__02
#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message) Pause(message)
#define PRINT_HEADING(message) PrintHeading(message)
#define PRINT_LINE(c) PrintLine(c)
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PAUSE(message)
#define PRINT_HEADING(c)
#define PRINT_LINE(c)
#endif