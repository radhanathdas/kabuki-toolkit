/** Kabuki Toolkit
@version 0.x
@file    ~/source/script/script_bin.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
All right reserved (R). Licensed under the Apache License, Version
2.0 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License
[here](http://www.apache.org/licenses/LICENSE-2.0). Unless
required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied. See the License for the specific language governing
permissions and limitations under the License.
*/

#include <stdafx.h>
#include "bin.h"
#include "type.h"
#include "bsq.h"
#include "hash.h"
#include "slot.h"
#include "console.h"
#include "text.h"

namespace _ {

#if USING_SCRIPT_TEXT
const char** BInState () {
    static const char* kStateStrings[] = {
        "Address",      //< 0
        "Args",         //< 1
        "UTF-8",        //< 2
        "UTF-16",       //< 3
        "UTF-32",       //< 4
        "Varint",       //< 5
        "Object",       //< 6
        "Hash",         //< 7
        "Error",        //< 8
        "Disconnected", //< 9
        "ACK",          //< 10
        "Locked",       //< 11
        "POD"           //< 12
    };
    return kStateStrings;
}
#endif

/** Used to return an erroneous result from a B-Output.
@param error The error type.
@return Returns a Static Error Op Result. */
inline const Op* BInError (BIn* bin, Error error) {
#if SCRIPT_DEBUG
    Text<> text;
    Display ("\nBIn " << ErrorString (error) << " Error!");
#endif
    return reinterpret_cast<const Op*> (error);
}

/** Used to return an erroneous result from a B-Output.
@param  bin    The source BIn.
@param  error   The error type.
@param  header  The B-Sequence Header.
@param  offset  The offset to the type in error in the B-Sequence.
@param  address The address of the byte in error.
@return         Returns a Static Error Op Result. */
inline const Op* BInError (BIn* bin, Error error,
                           const uint_t* header) {
#if SCRIPT_DEBUG
    Text<> text;
    Display ("\nBIn " << ErrorString (error) << " Error!");
#endif  //< MEMORY_PROFILE >= USE_MORE_ROM
    return reinterpret_cast<const Op*> (error);
}

/** Used to return an erroneous result from a B-Output.
@param  bin    The source BIn.
@param  error   The error type.
@param  header  The B-Sequence Header.
@param  offset  The offset to the type in error in the B-Sequence.
@param  address The address of the byte in error.
@return         Returns a Static Error Op Result. */
inline const Op* BInError (BIn* bin, Error error,
                           const uint_t* header,
                           uint_t offset) {
#if SCRIPT_DEBUG
    Text<> text;
    Display ("\nBIn " << ErrorString (error) << " Error!");
#endif  //< MEMORY_PROFILE >= USE_MORE_ROM
    return reinterpret_cast<const Op*> (error);
}

/** Used to return an erroneous result from a B-Output.
@param  bin    The source BIn.
@param  error   The error type.
@param  header  The B-Sequence Header.
@param  offset  The offset to the type in error in the B-Sequence.
@param  address The address of the byte in error.
@return         Returns a Static Error Op Result. */
inline const Op* BInError (BIn* bin, Error error,
                           const uint_t* header,
                           uint_t offset,
                           char* address) {
    Text<> text;
    Display ("\nBIn " << ErrorString (error) << " Error!");
    return reinterpret_cast<const Op*> (error);
}

BIn* BInInit (uintptr_t* buffer, uint_t size) {
    if (size < kMinSlotSize)
        return nullptr;
    if (buffer == nullptr)
        return nullptr;

    BIn* bin = reinterpret_cast<BIn*> (buffer);
    bin->size = size - sizeof (BIn);
    bin->start = 0;
    bin->stop = 0;
    bin->read = 0;

#if SCRIPT_DEBUG
    MemoryClear (BInBegin (bin), size);
#endif
    return bin;
}

int BInStreamByte (BIn* bin) {

    char* begin = BInBegin (bin),
        *end = begin + bin->size;
    char* open = (char*)begin + bin->read,
        *start = begin + bin->start,
        *cursor = start;

    int length = (int)(start < open) ? open - start + 1 :
        (end - start) + (open - begin) + 2;

    if (length < 1) {
        BInError (bin, kErrorBufferOverflow, Bsq<1, STR> (), 2, start);
        return -1;
    }
    //byte b = *cursor;
    bin->stop = (++cursor >= end) ? static_cast<uint_t> (MemoryVector (begin, end)) :
        static_cast<uint_t> (MemoryVector (begin, cursor));
    return 0;
}

bool BInIsReadable (BIn* bin) {
    return BinBufferLength (bin) > 0;
}

const Op* BInRead (BIn* bin, const uint_t* params, void** args) {
#if SCRIPT_DEBUG
    Text<> text;
    Display ("\nReading " << text.Bsq (params)
          << " from B-Input:" << text.Pointer (bin));
    Write (BInPrint (bin, text));
#endif
    if (!bin) {
        return BInError (bin, kErrorImplementation);
    }
    if (!params) {
        return BInError (bin, kErrorImplementation);
    }
    if (!args) {
        return BInError (bin, kErrorImplementation);
    }
    byte      ui1;                  //< Temp variable.
    uint16_t  ui2;                  //< Temp variable.
    uint32_t  ui4;                  //< Temp variable.
    uint64_t  ui8;                  //< Temp variable.
    char*     ui1_ptr;              //< Pointer to a UI1.
    uint16_t* ui2_ptr;              //< Pointer to a UI2.
    uint32_t* ui4_ptr;              //< Pointer to a UI4.
    uint64_t* ui8_ptr;              //< Pointer to a UI1.
    uint_t    type,                 //< The current type being read.
        size,                 //< Size of the ring buffer.
        length,               //< Length of the data in the buffer.
        count,                //< Argument length.
        index,                //< Index in the params.
        arg_index = 0,        //< Index in the args.
        value,                 //< Temp variable.
        num_params = *params; //< Number of params.
    hash16_t  hash;                 //< Hash of the incoming data.

    if (num_params == 0) return 0;  //< Nothing to do.

    hash = kLargest16BitPrime;
    size = bin->size;

    char* begin = BInBegin (bin),     //< The beginning of the buffer.
        *end = begin + size,       //< The end of the buffer.
        *start = begin + bin->start, //< The start of the data.
        *stop = begin + bin->stop;  //< The stop of the data.
                                    //const uint_t* param = params + 1; //< The current param.

    length = SlotLength (start, stop, size);

    // When we scan, we are reading from the beginning of the Slot buffer.

    for (index = 1; index <= num_params; ++index) {
        type = params[index];
    #if SCRIPT_DEBUG == SCRIPT_BIN
        Display ("\nparam:" << arg_index + 1
              << " type:" << TypeString (type)
              << " start:" << MemoryVector (begin, start)
              << " stop:" << MemoryVector (begin, stop)
              << " length:" << length);
    #endif  //< SCRIPT_DEBUG == SCRIPT_BIN
        switch (type) {
            case NIL:
                return BInError (bin, kErrorInvalidType, params, index,
                                 start);
            case ADR:
            case STR: //< _R_e_a_d__S_t_r_i_n_g_-_8_____________________________
                      // Load buffered-type argument length and increment the index.
                ++num_params;
                count = params[++index];
                //< @todo Replace with pointer arithmetic.
                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<char*> (args[arg_index]);
                if (ui1_ptr == nullptr)
                    return BInError (bin, kErrorImplementation, params, index,
                                     start);
            #if SCRIPT_DEBUG
                Display ("\nReading STR:0x" << text.Pointer (ui1_ptr)
                      << " with max length:" << count);
            #endif
                // Read char.
                ui1 = *start;
                hash = Hash16 (ui1, hash);
                if (++start >= end) start -= size;
                *ui1_ptr = ui1;
                ++ui1_ptr;
            #if SCRIPT_DEBUG
                Write (ui1);
            #endif
                while ((ui1 != 0) && (count != 0)) {
                    --count;
                    if (count == 0) //< Reached count:0 before nil-term char.
                        return BInError (bin, kErrorBufferUnderflow,
                                         params, index, start);
                    ui1 = *start;       // Read byte from ring-buffer.
                    hash = Hash16 (ui1, hash);
                    if (++start >= end) start -= size;
                    *ui1_ptr++ = ui1;   // Write byte to destination.
                #if SCRIPT_DEBUG
                    Write (ui1);
                #endif
                }
            #if SCRIPT_DEBUG
                Write ("\" success!\n");
            #endif
                if (type != ADR) {
                    *ui1_ptr = 0;
                    // No need to hash 0.
                }
                break;
            case SI1: //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_______________________
            case UI1:
            case BOL:
            #if USING_SCRIPT_1_BYTE_TYPES
                if (length-- == 0)
                    return BInError (bin, kErrorBufferUnderflow, params,
                                     index, start);

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<char*> (args[arg_index]);
                if (!ui1_ptr)
                    break;

                // Read type;

                // Byte 1
                ui1 = *start;                       //< Read
                #if SCRIPT_DEBUG
                Display (" \'" << ui1 << "\',");
                #endif  //< SCRIPT_DEBUG
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *ui1_ptr = ui1;                     //< Write
                break;
                #else
                return BInError (bin, kErrorInvalidType, params,
                                 index, start);
                #endif
            case SI2: //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s______________________
            case UI2:
            case HLF:
                #if USING_SCRIPT_2_BYTE_TYPES
                if (length < 2)
                    return BInError (bin, kErrorBufferUnderflow, params,
                                     index, start);
                length -= 2;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<char*> (args[arg_index]);
                if (!ui1_ptr) break;

                // Read type

                // Byte 1
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *ui1_ptr = ui1;                     //< Write

                                                    // Byte 2
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 1) = ui1;               //< Write
                break;
                #else
                return BInError (bin, kErrorInvalidType, params,
                                 index, start);
                #endif
            case SVI: //< _R_e_a_d__S_i_g_n_e_d__V_a_r_i_n_t____________________
            case UVI: //< _R_e_a_d__U_n_s_i_g_n_e_d__V_a_r_i_n_t________________
            #if WORD_SIZE <= 16
                      // Load next pointer and increment args.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr)
                    return BInError (bin, kErrorImplementation, params, index,
                                     start);

                // Scan byte 1.
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui2 = ui1;
                temp = 7;        //< Number of bits to shift ui1 to the left.
                count = 5; //< The max number of Varint4 bytes.
                while (ui1 >> 7 == 0) {
                    if (length-- == 0)
                        return BInError (bin, kErrorBufferUnderflow,
                                         params, index, start);
                    ui1 = *start;
                    if (++start >= end) start -= size;
                    hash = Hash16 (ui1, hash);
                    ui4 |= ((uint32_t)(ui1 & 0x7F)) << temp;
                    //< @todo I'm starting to second guess if we need to mask ui1 
                    //< because we're packing them up and will overwrite.
                    temp += 7;
                    if (--count == 0)
                        return BInError (bin, kErrorVarintOverflow, params,
                                         index, start);
                }
                if (count == 5)    //< If there is only one byte we need to
                    ui4 &= 0x7F;        //< mask off the terminating varint bit.
                if (type == SVI)
                    ui4 = TypeUnpackVarint<uint32_t> (ui4);
                *ui4_ptr = ui4;
                break;
                #else
                      // Load next pointer and increment args.
                ui4_ptr = reinterpret_cast<uint32_t*> (args[arg_index]);
                if (ui4_ptr == nullptr)
                    return BInError (bin, kErrorImplementation, params, index,
                                     start);

                // Scan byte 1.
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui4 = ui1;
                ui2 = 7;    //< Number of bits to shift ui1 to the left.
                count = 5;  //< The max number of Varint4 bytes.
                while (ui1 >> 7 == 0) {
                    if (length-- == 0)
                        return BInError (bin, kErrorBufferUnderflow,
                                         params, index, start);
                    ui1 = *start;
                    if (++start >= end) start -= size;
                    hash = Hash16 (ui1, hash);
                    ui4 |= ((uint32_t)(ui1 & 0x7F)) << ui2;
                    //< @todo I'm starting to second guess if we need to mask ui1 
                    //< because we're packing them up and will overwrite.
                    ui2 += 7;
                    if (--count == 0)
                        return BInError (bin, kErrorVarintOverflow, params,
                                         index, start);
                }
                if (count == 5)    //< If there is only one byte we need to
                    ui4 &= 0x7F;        //< mask off the terminating varint bit.
                if (type == SVI)
                    ui4 = TypeUnpackVarint<uint32_t> (ui4);
                *ui4_ptr = ui4;
                break;
                #endif
            case SI4: //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s______________________
            case UI4:
            case FLT:
            case TMS:
                #if USING_SCRIPT_4_BYTE_TYPES
                if (length < 4)
                    return BInError (bin, kErrorBufferUnderflow, params,
                                     index, start);
                length -= 4;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<char*> (args[arg_index]);
                if (!ui1_ptr) break;

                // Read type

                for (value = sizeof (uint32_t); value > 0; --value) {
                    // Byte 1
                    ui1 = *start;                       //< Read
                    hash = Hash16 (ui1, hash);          //< Hash
                    if (++start >= end) start -= size;  //< Increment
                    *ui1_ptr++ = ui1;                   //< Write
                }
                #else
                return BInError (bin, kErrorInvalidType, params,
                                 index, start);
                #endif
            case TMU: //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s______________________
            case SI8:
            case UI8:
            case DBL:
                #if USING_SCRIPT_8_BYTE_TYPES
                if (length < 8)
                    return BInError (bin, kErrorBufferUnderflow, params,
                                     index, start);
                length -= 8;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<char*> (args[arg_index]);
                if (!ui1_ptr) break;

                // Read type
                for (value = sizeof (uint64_t); value > 0; --value) {
                    // Byte 1
                    ui1 = *start;                       //< Read
                    hash = Hash16 (ui1, hash);          //< Hash
                    if (++start >= end) start -= size;  //< Increment
                    *ui1_ptr++ = ui1;                     //< Write
                }
                break;
                #else
                return BInError (bin, kErrorInvalidType, params,
                                 index, start);
                #endif
            case SV8: //< _R_e_a_d__V_a_r_i_n_t__8______________________________
            case UV8:
                // Load next pointer and increment args.
                ui8_ptr = reinterpret_cast<uint64_t*> (args[arg_index]);
                if (ui8_ptr == nullptr)
                    return BInError (bin, kErrorImplementation, params, index,
                                     start);

                // Scan byte 1.
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui8 = ui1;
                ui2 = 7;        //< Number of bits to shift ui1 to the left.
                count = 9; //< The max number of Varint8 bytes.
                while (ui1 >> 7 == 0) {
                    if (length-- == 0)
                        return BInError (bin, kErrorBufferUnderflow,
                                         params, index, start);
                    ui1 = *start;
                    if (++start >= end) start -= size;
                    hash = Hash16 (ui1, hash);
                    if (count == 1) {
                        // Varint 8 differs from Varint 2 and 4 in that on the 
                        // last byte, byte 9 of 9, there is no terminating 
                        // varint bit.
                        ui8 |= ((uint64_t)(ui1)) << ui2;
                        break;
                    }
                    ui8 |= ((uint64_t)(ui1 & 0x7F)) << ui2;
                    //< @todo I'm starting to second guess if we need to mask 
                    //< ui1 because we're packing them up and will overwrite.
                    ui2 += 7;
                    if (--count == 0)
                        return BInError (bin, kErrorVarintOverflow, params,
                                         index, start);
                }
                if (count == 9)    //< If there is only one byte we need to
                    ui8 &= 0x7F;        //< mask off the terminating varint bit.
                if (type == SV8)
                    ui8 = TypeUnpackVarint<uint64_t> (ui8);
                *ui8_ptr = ui8;
                break;
            case BSQ: //< _B_-_S_e_q_u_e_n_c_e__S_t_r_i_n_g_____________________
                #if USING_BSC
                ui1_ptr = reinterpret_cast<char*> (args[arg_index]);
                if (ui1_ptr == nullptr)
                    return BInError (bin, kErrorImplementation, params, index,
                                     start);
                ui1 = *start;
                #endif 
            default: {  //< It's an Array
                #if SCRIPT_DEBUG
                std::cout << "\nIt's an array!\n";
                #endif  //< SCRIPT_DEBUG
                #if USING_SCRIPT_ARRAY
                switch (type & 0x60) {
                    case 0: {
                        if ((type < LST) && (type < MAP))
                            return BInError (bin, kErrorInvalidType, params,
                                             index, start);
                        if (length < 1) // 1 byte for the width word.
                            return BInError (bin, kErrorBufferUnderflow,
                                             params, index, start);

                        ui1_ptr = reinterpret_cast<char*> (args[arg_index]);
                        if (ui1_ptr == nullptr)
                            return BInError (bin, kErrorImplementation, params,
                                             index, start);

                        ui1 = *start;
                        if (++start >= end) start -= size;
                        hash = Hash16 (ui1, hash);
                        if (ui1 > length - 1)
                            return BInError (bin, kErrorBufferOverflow,
                                             params, index, start);
                        length = length - count - 1;
                        count = (uintptr_t)ui1;
                        break;
                    }
                    case 1: {
                        if (length < 2) // 2 byte for the width word.
                            return BInError (bin, kErrorBufferUnderflow,
                                             params, index, start);
                        length -= 2;
                        ui2_ptr = reinterpret_cast<uint16_t*>
                            (args[arg_index]);
                        if (ui2_ptr == nullptr)
                            return BInError (bin, kErrorImplementation, params,
                                             index, start);

                        for (temp = 0; temp <= sizeof (uint16_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui2 |= ((uint16_t)ui1) << temp;
                        }
                        if (ui2 > length)
                            return BInError (bin, kErrorBufferOverflow,
                                             params, index, start);
                        length -= count;
                        count = (uintptr_t)ui2;
                        ui1_ptr = reinterpret_cast<char*> (ui2_ptr);
                        break;
                    }
                    case 2: {
                        if (length < 4) // 4 byte for the width word.
                            return BInError (bin, kErrorBufferUnderflow,
                                             params, index, start);
                        length -= 4;
                        ui4_ptr = reinterpret_cast<uint32_t*> (args[arg_index]);
                        if (ui4_ptr == nullptr)
                            return BInError (bin, kErrorImplementation, params,
                                             index, start);

                        for (temp = 0; temp <= sizeof (uint32_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui4 |= ((uint32_t)ui1) << temp;
                        }
                        if (ui4 >= length)
                            return BInError (bin, kErrorBufferOverflow,
                                             params, index, start);
                        length -= count;
                        count = (uintptr_t)ui4;
                        ui1_ptr = reinterpret_cast<char*> (ui4_ptr);
                        break;
                    }
                    case 3: { // 8 byte for the width word.
                        if (length < 9)
                            return BInError (bin, kErrorBufferUnderflow,
                                             params, index, start);
                        length -= 8;
                        ui8_ptr = reinterpret_cast<uint64_t*> (args[arg_index]);
                        if (ui8_ptr == nullptr)
                            return BInError (bin, kErrorImplementation, params,
                                             index, start);

                        for (temp = 0; temp <= sizeof (uint64_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui8 |= ((uint64_t)ui1) << temp;
                        }
                        if (ui8 > length)
                            return BInError (bin, kErrorBufferOverflow,
                                             params, index, start);
                        length -= count;
                        count = (uintptr_t)ui8;
                        ui1_ptr = reinterpret_cast<char*> (ui8_ptr);
                        break;

                    }
                    default: return BInError (bin, kErrorImplementation, params,
                                              index, start);
                }

                if (length < count)
                    return BInError (bin, kErrorBufferOverflow, params,
                                     index, start);
                if (count == 0)
                    break;          //< Not sure if this is an error.
                if (start + count >= end) {
                    for (; size - count > 0; --count) {
                        ui1 = *start;
                        if (++start >= end) start -= size;
                        hash = Hash16 (ui1, hash);
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    stop = begin - 1;
                    for (; count > 0; --count) {
                        ui1 = *start;
                        if (++start >= end) start -= size;
                        hash = Hash16 (ui1, hash);
                        *ui1_ptr = ui1;
                        ++ui1_ptr;
                    }
                    break;
                }
                for (; count > 0; --count) {
                    ui1 = *start;
                    if (++start >= end) start -= size;
                    hash = Hash16 (ui1, hash);
                    *ui1_ptr = ui1;
                    ++ui1_ptr;
                }
                break;
            #endif  //< SCRIPT_DEBUG    
            }
        }
        ++arg_index;
        #if SCRIPT_DEBUG
        std::cout << " |";
        #endif  //< SCRIPT_DEBUG    
    }
    #if SCRIPT_DEBUG
    Write ("\nHash expected:") << text.Hex (hash));
    #endif
    if (length < 2)
        return BInError (bin, kErrorBufferUnderflow, params, index,
                         start);
    ui2 = *start;
    if (++start >= end) start -= size;
    ui1 = *start;
    if (++start >= end) start -= size;
    ui2 |= (((uint16_t)ui1) << 8);
    #if SCRIPT_DEBUG
    Display ("found:" << text.Hex (ui2));
    #endif
    if (hash != ui2)
        return BInError (bin, kErrorInvalidHash, params, index,
                         start);

    //#if SCRIPT_DEBUG
    //    //printf ("\nDone reading\n");
    //    SlotClear (begin, bin->start, start, stop, end, size);
    //#endif

    // Convert pointer back to offset
    bin->start = (uint_t)MemoryVector (begin, start);

    return 0;
}

#if USING_SCRIPT_TEXT
Slot& BInPrint (BIn* bin, Slot& slot) {
    if (!bin) {
        return slot << "\nError: BIn can't be nil";
    }
    uint_t size = bin->size;
    return slot.Line ('_')
        << "\nBIn:" << slot.Pointer (bin) << " size:" << bin->size
        << " start:" << bin->start << " stop:" << bin->stop
        << " read:" << bin->read
        << slot.Memory (BInBegin (bin), size + sizeof (BIn));
}
#endif  //< USING_SCRIPT_TEXT

}       //< namespace _

#undef DEBUG_SCRIPT_BIN
