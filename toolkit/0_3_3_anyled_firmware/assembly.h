/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/assembly.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
    
    @desc   This file contains the system configuration for ChineseRoom and  
            applications and libraries that use it. To modify this file, copy 
            it into your project directory into the source code root. This 
            files gets #include <assembly.h> in the config file. That means 
            that the compiler will look in your source code root before looking 
            in this file. Please see module_config.h for configuration details.
*/

// Choose BARE_METAL, MBED, MBED_OS, ARDUINO, MINGW32, WINDOWS, ANDROID,
// LINUX, OSX, or IOS
#define PLATFORM            MBED

// Choose X86, X64, ARM8, ARM16, ARM32, or ARM64
#define CPU_ACHITECTURE     X86

#define CPU_WORD_SIZE       32

#define IS_LITTLE_ENDIAN    YES

#define DEBUG               YES
#define SCRIPT_DEBUG        NO

/** @brief Memory Profiles 1-4 are used to signal that the system is
           memory constrained.
    For constrained memory devices, use MEMORY_PROFILE 1. For most uses use
    MEMORY_PROFILE 2. For applications that require 64-bit memory addresses, use
    MEMORY_PROFILE 3.

    @code
    | MUL | sizeof (uint_t) | #bits Unit |
    |:---:|:---------------:|:----------:|
    |  1  |      1 byte     |     5      |
    |  2  |      2 byte     |    10      |
    |  4  |      4 byte     |    16      |
    @endcode
*/
#define MEMORY_PROFILE      2        //< Memory Use Level 1-5.

#define MAX_ERRORS          8         //< Max errors before locking up.
#define MAX_NUM_PARAMS      26        //< Max number of parameters.
#define MAX_STRING_LENGTH   64 * 1024 //< Max string length.
#define MAX_NUM_SLOTS       256       //< Max number of Slots.
#define COM_TIMEOUT_TICKS   100       //< Number of seconds before a timeout.
                                      //< over a generic communication link.
#define CHAR_WIDTH          8         //< char_t width in bytes: 8, 16, or 32.
#define ROOM_FLOOR_SIZE     1024      //< Room floor wall size.

// Sets any of the following macros to YES or NO.

#define USING_1_BYTE_TYPES  YES
#define USING_2_BYTE_TYPES  YES
#define USING_4_BYTE_TYPES  NO
#define USING_8_BYTE_TYPES  NO
#define USING_VARINT2       NO
#define USING_VARINT4       NO
#define USING_VARINT8       NO
#define USING_ARRAY         NO
#define USING_BAG           NO
#define USING_BOOK          NO
#define USING_MAP           NO
#define USING_LIST          NO
