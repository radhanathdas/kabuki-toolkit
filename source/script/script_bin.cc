/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_bin.cc
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
*/

#include <stdafx.h>
#include "bin.h"
#include "types.h"
#include "params.h"
#include "hash.h"
#include "slot.h"

using namespace std;

namespace _ {

#if SCRIPT_USING_TEXT
const char** BinState () {
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
    @return Returns a Static Error Operation Result. */
inline const Operation* ErrorReport (Bin* bin, Error error) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bin " << ErrorString (error) << " Error!";
#endif
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bin    The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* ErrorReport (Bin* bin, Error error,
                                    const uint_t* header) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bin " << ErrorString (error) << " Error!";
#endif  //< MEMORY_PROFILE >= USE_MORE_ROM
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bin    The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* ErrorReport (Bin* bin, Error error,
                                    const uint_t* header,
                                    uint_t offset) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bin " << ErrorString (error) << " Error!";
#endif  //< MEMORY_PROFILE >= USE_MORE_ROM
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bin    The source Bin.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* ErrorReport (Bin* bin, Error error,
                                    const uint_t* header,
                                    uint_t offset,
                                    byte* address) {
    std::cout << "\n| Bin " << ErrorString (error) << " Error!";
    return reinterpret_cast<const Operation*> (1);
}

byte* BinBuffer (Bin* bin) {
    if (!bin)
        return nullptr;
    byte* ptr = reinterpret_cast<byte*> (bin);
    return ptr + sizeof (Bin);
}

Bin* BinInit (uintptr_t* buffer, uint_t size) {
    if (size < kMinSlotSize)
        return nullptr;
    if (buffer == nullptr)
        return nullptr;

    Bin* bin   = reinterpret_cast<Bin*> (buffer);
    bin->size  = size - sizeof (Bin);
    bin->start = 0;
    bin->stop  = 0;
    bin->read  = 0;

#if SCRIPT_DEBUG
    MemoryClear (BinBuffer (bin), size);
#endif
    return bin;
}

uint_t BinSpace (Bin* bin) {
    if (!bin) return ~0;
    byte* txb_ptr = reinterpret_cast<byte*>(bin);
    return SlotSpace (txb_ptr + bin->start, txb_ptr + bin->stop,
        bin->size);
}

uint_t BinBufferLength (Bin* bin) {
    if (!bin) return ~0;
    byte* base = BinBuffer (bin);
    return SlotLength (base + bin->start, base + bin->stop, bin->size);
}

byte* BinEndAddress (Bin* bin) {
    if (!bin) {
        return nullptr;
    }
    return reinterpret_cast<byte*>(bin) + (4 * sizeof (uint_t)) + bin->size;
}

int BinStreamByte (Bin* bin) {

    byte* begin = BinBuffer (bin),
        *end = begin + bin->size;
    byte* open = (byte*)begin + bin->read,
        *start = begin + bin->start,
        *cursor = start;

    int length = (int)(start < open) ? open - start + 1:
        (end - start) + (open - begin) + 2;

    if (length < 1) {
        ErrorReport (bin, kErrorBufferOverflow, Params<1, STR> (), 2, start);
        return -1;
    }
    //byte b = *cursor;
    bin->stop = (++cursor >= end)?static_cast<uint_t> (Diff (begin, end)):
        static_cast<uint_t> (Diff (begin, cursor));
    return 0;
}

bool BinIsReadable (Bin* bin) {
    return BinBufferLength (bin) > 0;
}

const Operation* BinRead (Bin* bin, const uint_t* params, void** args) {
#if SCRIPT_DEBUG
    Text text;
    cout << "\n| Reading " << ParamsPrint (params, text)
        << " from B-Input:" << text.Pointer (bin)
        << BinPrint (bin, text);
    Dump (text);
#endif
    if (!bin)
        return ErrorReport (bin, kErrorImplementation);
    if (!params)
        return ErrorReport (bin, kErrorImplementation);
    if (!args)
        return ErrorReport (bin, kErrorImplementation);
    byte     //array_type,            //< The current type being read.
             ui1;                     //< Temp variable.
    uint16_t ui2;                     //< Temp variable.
#if SCRIPT_USING_VARINT4
    uint32_t ui4;
#endif
#if SCRIPT_USING_VARINT8
    uint64_t ui8;
#endif

    byte*     ui1_ptr;              //< Pointer to a UI1.
#if  SCRIPT_USING_VARINT2
    uint16_t* ui2_ptr;              //< Pointer to a UI2.
#endif
#if SCRIPT_USING_VARINT4
    uint32_t* ui4_ptr;              //< Pointer to a UI4.
#endif
#if SCRIPT_USING_VARINT8
    uint64_t* ui8_ptr;              //< Pointer to a UI1.
#endif
    uint_t type,                    //< The current type being read.
           size,                    //< Size of the ring buffer.
           length,                  //< Length of the data in the buffer.
           count,                   //< Argument length.
           index,                   //< Index in the params.
           arg_index = 0,           //< Index in the args.
#if SCRIPT_USING_ARRAY
           temp,
#endif  //< SCRIPT_USING_ARRAY
           num_params = *params;    //< Number of params.
    hash16_t hash;

    if (num_params == 0) return 0;  //< Nothing to do.

    hash = kLargest16BitPrime;
    size = bin->size;

    byte* begin = BinBuffer (bin),    //< The beginning of the buffer.
        * end   = begin + size,       //< The end of the buffer.
        * start = begin + bin->start, //< The start of the data.
        * stop  = begin + bin->stop;  //< The stop of the data.
    //const uint_t* param = params + 1; //< The current param.

    length = SlotLength (start, stop, size);
    
    // When we scan, we are reading from the beginning of the Slot buffer.

    for (index = 1; index <= num_params; ++index) {
        type   = params[index];
#if SCRIPT_DEBUG
        Dump (text << "\n| param:" << arg_index + 1
                   << " type:"     << TypeString (type)
                   << " start:"    << Diff (begin, start)
                   << " stop:"     << Diff (begin, stop)
                   << " length:"   << length);
#endif
        switch (type) {
            case NIL:
                return ErrorReport (bin, kErrorInvalidType, params, index,
                                  start);
            case ADR:
            case STR: //< _R_e_a_d__S_t_r_i_n_g_-_8_____________________________
                // Load buffered-type argument length and increment the index.
                ++num_params;
                count = params[++index];
                //< @todo Replace with pointer arithmetic.
                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                if (ui1_ptr == nullptr)
                    return ErrorReport (bin, kErrorImplementation, params, index, 
                                      start);
#if SCRIPT_DEBUG
                text << "\n| Reading STR:0x" << text.Pointer (ui1_ptr) << " with max "
                        "length:" << count;
#endif
                // Read char.
                ui1 = *start;
                hash = Hash16 (ui1, hash);
                if (++start >= end) start -= size;
                *ui1_ptr = ui1;
                ++ui1_ptr;
#if SCRIPT_DEBUG
                std::cout << ui1;
#endif
                while ((ui1 != 0) && (count != 0)) {
                    --count;
                    if (count == 0) //< Reached count:0 before nil-term char.
                        return ErrorReport (bin, kErrorBufferUnderflow,
                                          params, index, start);
                    ui1 = *start;       // Read byte from ring-buffer.
                    hash = Hash16 (ui1, hash);
                    if (++start >= end) start -= size;
                    *ui1_ptr++ = ui1;   // Write byte to destination.
#if SCRIPT_DEBUG
                    std::cout << ui1;
#endif
                }
#if SCRIPT_DEBUG
                std::cout << "\" success!\n";
#endif
                if (type != ADR) {
                    *ui1_ptr = 0;
                    // No need to hash 0.
                }
                break;
            case ST2: //< _R_e_a_d__S_t_r_i_n_g_-_16____________________________
#if USING_ST2
                // Load buffered-type argument length and increment the index.
                ++num_params;
                count = params[++index];
                //#if SCRIPT_DEBUG
                //printf ("\n|           Reading char with max length %u: ",
                //        count);
                //#endif
                // Load next pointer and increment args.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr) break;

                // Read char.
                ui2 = *start;
                hash = Hash16UI2 (ui2, hash);
                if (++start >= end) start -= size;
                *ui2_ptr = ui2;
                ++ui2_ptr;

                while (ui2 != 0 && count != 0) {
                    if (count == 1)
                        return ErrorReport (bin, kErrorBufferUnderflow, 
                                          params, index, start);
                    --count;
                    //#if SCRIPT_DEBUG
                    //std::cout << ui2;
                    //#endif
                    ui2 = *start;       // Read byte from ring-buffer.
                    hash = Hash16UI2 (ui2, hash);
                    if (++start >= end) start -= size;
                    *ui2_ptr++ = ui2;   // Write byte to destination.
                }
                //#if SCRIPT_DEBUG
                //            //printf (" done!\n");
                //#endif
                hash = Hash16UI2 (ui2, hash);
                *ui2_ptr = ui2;
                break;
#else
                return ErrorReport (bin, kErrorInvalidType, params, 
                                  index, start);
#endif
            case SI1: //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s_______________________
            case UI1:
            case BOL:
#if SCRIPT_USING_1_BYTE_TYPES
                if (length-- == 0)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                if (ui1_ptr == 0)
                    break;

                // Read type;

                // Byte 1
                ui1 = *start;                       //< Read
#if SCRIPT_DEBUG
                std::cout << " \'" << ui1 << "\',";
#endif  //< SCRIPT_DEBUG
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *ui1_ptr = ui1;                     //< Write
                break;
#else
                return ErrorReport (bin, kErrorInvalidType, params, 
                                  index, start);
#endif
            case SI2: //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s______________________
            case UI2:
            case HLF:
#if SCRIPT_USING_2_BYTE_TYPES
                if (length < 2)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);
                length -= 2;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                if (ui1_ptr == 0) break;

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
                return ErrorReport (bin, kErrorInvalidType, params, 
                                  index, start);
#endif
            case SI4: //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s______________________
            case UI4:
            case FLT:
            case TMS:
#if SCRIPT_USING_4_BYTE_TYPES
                if (length < 4)
                    return ErrorReport (bin, kErrorBufferUnderflow, params, 
                                      index, start);
                length -= 4;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                if (ui1_ptr == 0) break;

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

                                                    // Byte 3
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 2) = ui1;               //< Write

                                                    // Byte 4
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 3) = ui1;               //< Write
                break;
#else
                return ErrorReport (bin, kErrorInvalidType, params,
                                  index, start);
#endif
            case TMU: //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s______________________
            case SI8:
            case UI8:
            case DBL:
#if SCRIPT_USING_8_BYTE_TYPES
                if (length < 8)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);
                length -= 8;

                // Load next pointer and increment args.
                ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                if (ui1_ptr == 0) break;

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

                                                    // Byte 3
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 2) = ui1;               //< Write

                                                    // Byte 4
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 3) = ui1;               //< Write

                                                    // Byte 5
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 4) = ui1;               //< Write

                                                    // Byte 6
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 5) = ui1;               //< Write

                                                    // Byte 7
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 6) = ui1;               //< Write

                                                    // Byte 8
                ui1 = *start;                       //< Read
                hash = Hash16 (ui1, hash);          //< Hash
                if (++start >= end) start -= size;  //< Increment
                *(ui1_ptr + 7) = ui1;               //< Write
                break;
#else
                return ErrorReport (bin, kErrorInvalidType, params,
                                  index, start);
#endif
            case SV2: //< _R_e_a_d__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______

#if SCRIPT_USING_VARINT2
                      // Load next pointer and increment args.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr)
                    return ErrorReport (bin, kErrorImplementation, params, index,
                                      start);

                // Read byte 1
                if (length-- == 0)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) != 0) {
                    ui2 = ui1 & 0x7F;   //< Mask off end-of-varint MSb.
                    *ui2_ptr = UnpackSignedVarint<uint16_t> (ui2);
                    break;
                }
                ui2 = ui1;

                // Read byte 2
                if (--length == 0)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) != 0) {
                    // Mask off end-of-varint MSb.
                    ui2 |= ((uint16_t)(ui1 & 0x7F)) << 7;
                    *ui2_ptr = UnpackSignedVarint<uint16_t> (ui2);
                    break;
                }
                ui2 |= ((uint16_t)ui1) << (2 * 7);

                // Read byte 3
                if (--length == 0)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) == 0)
                    return ErrorReport (bin, kErrorVarintOverflow, params,
                                      index, start);
                ui2 |= ((uint16_t)(ui1 & 0x7F) << 14);
                *ui2_ptr = UnpackSignedVarint<uint16_t> (ui2);
                break;

            case UV2: //< _R_e_a_d__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___
                      // Load next pointer and increment args.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr)
                    return ErrorReport (bin, kErrorImplementation, params, index,
                                      start);

                // Read byte 1
                if (length-- == 0)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) != 0) {
                    ui2 = ui1 & 0x7F;   //< Mask off end-of-varint MSb.
                    *ui2_ptr = ui2;
                    break;
                }
                ui2 = ui1;

                // Read byte 2
                if (--length == 0)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) != 0) {
                    // Mask off end-of-varint MSb.
                    ui2 |= ((uint16_t)(ui1 & 0x7F)) << 7;
                    *ui2_ptr = ui2;
                    break;
                }
                ui2 |= ((uint16_t)ui1) << (2 * 7);

                // Read byte 3
                if (--length == 0)
                    return ErrorReport (bin, kErrorBufferUnderflow, params,
                                      index, start);
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                if ((ui1 >> 7) == 0)
                    return ErrorReport (bin, kErrorVarintOverflow, params,
                                      index, start);
                ui2 |= ((uint16_t)(ui1 & 0x7F) << 14);
                *ui2_ptr = ui2;
                break;
#else
            case UV2:
                return ErrorReport (bin, kErrorInvalidType, params, index,
                                  start);
#endif
            case SV4: //< _R_e_a_d__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_______
            case UV4: //< _R_e_a_d__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t___
#if SCRIPT_USING_VARINT4
                      // Load next pointer and increment args.
                ui4_ptr = reinterpret_cast<uint32_t*> (args[arg_index]);
                if (ui4_ptr == nullptr)
                    return ErrorReport (bin, kErrorImplementation, params, index,
                                      start);

                // Scan byte 1.
                ui1 = *start;
                if (++start >= end) start -= size;
                hash = Hash16 (ui1, hash);
                ui4 = ui1;
                ui2 = 7;        //< Number of bits to shift ui1 to the left.
                count = 5; //< The max number of Varint4 bytes.
                while (ui1 >> 7 == 0) {
                    if (length-- == 0)
                        return ErrorReport (bin, kErrorBufferUnderflow,
                                          params, index, start);
                    ui1 = *start;
                    if (++start >= end) start -= size;
                    hash = Hash16 (ui1, hash);
                    ui4 |= ((uint32_t)(ui1 & 0x7F)) << ui2;
                    //< @todo I'm starting to second guess if we need to mask ui1 
                    //< because we're packing them up and will overwrite.
                    ui2 += 7;
                    if (--count == 0)
                        return ErrorReport (bin, kErrorVarintOverflow, params,
                                          index, start);
                }
                if (count == 5)    //< If there is only one byte we need to
                    ui4 &= 0x7F;        //< mask off the terminating varint bit.
                if (type == SV4)
                    ui4 = UnpackSignedVarint<uint32_t> (ui4);
                *ui4_ptr = ui4;
                break;
#else
                return ErrorReport (bin, kErrorInvalidType, params, index,
                                  start);
#endif
            case SV8: //< _R_e_a_d__V_a_r_i_n_t__8______________________________
            case UV8:
#if SCRIPT_USING_VARINT8
                // Load next pointer and increment args.
                ui8_ptr = reinterpret_cast<uint64_t*> (args[arg_index]);
                if (ui8_ptr == nullptr)
                    return ErrorReport (bin, kErrorImplementation, params, index,
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
                        return ErrorReport (bin, kErrorBufferUnderflow,
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
                        return ErrorReport (bin, kErrorVarintOverflow, params,
                                          index, start);
                }
                if (count == 9)    //< If there is only one byte we need to
                    ui8 &= 0x7F;        //< mask off the terminating varint bit.
                if (type == SV8)
                    ui8 = UnpackSignedVarint<uint64_t> (ui8);
                *ui8_ptr = ui8;
                break;
#else
                return ErrorReport (bin, kErrorInvalidType, params, index,
                                  start);
#endif
            case BSC: //< _B_-_S_e_q_u_e_n_c_e__S_t_r_i_n_g_____________________
#if USING_BSC
            ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
            if (ui1_ptr == nullptr)
                return ErrorReport (bin, kErrorImplementation, params, index,
                                  start);
            ui1 = *start;
#endif 
            default: {  //< It's an Array
#if SCRIPT_DEBUG
                std::cout << "\nIt's an array!\n";
#endif  //< SCRIPT_DEBUG
#if SCRIPT_USING_ARRAY
                switch ((type >> 5) & 0x3) {
                    case 0: {
                        if ((type < LST) && (type < MAP))
                            return ErrorReport (bin, kErrorInvalidType, params, 
                                              index, start);
                        if (length < 1) // 1 byte for the width word.
                            return ErrorReport (bin, kErrorBufferUnderflow,
                                              params, index, start);

                        ui1_ptr = reinterpret_cast<byte*> (args[arg_index]);
                        if (ui1_ptr == nullptr)
                            return ErrorReport (bin, kErrorImplementation, params,
                                              index, start);

                        ui1 = *start;
                        if (++start >= end) start -= size;
                        hash = Hash16 (ui1, hash);
                        if (ui1 > length - 1)
                            return ErrorReport (bin, kErrorBufferOverflow,
                                              params, index, start);
                        length = length - count - 1;
                        count = (uintptr_t)ui1;
                        break;
                    }
                    case 1: {
                        if (length < 2) // 2 byte for the width word.
                            return ErrorReport (bin, kErrorBufferUnderflow,
                                              params, index, start);
                        length -= 2;
                        ui2_ptr = reinterpret_cast<uint16_t*> 
                                    (args[arg_index]);
                        if (ui2_ptr == nullptr)
                            return ErrorReport (bin, kErrorImplementation, params,
                                              index, start);

                        for (temp = 0; temp <= sizeof (uint16_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui2 |= ((uint16_t)ui1) << temp;
                        }
                        if (ui2 > length)
                            return ErrorReport (bin, kErrorBufferOverflow,
                                              params, index, start);
                        length -= count;
                        count = (uintptr_t)ui2;
                        ui1_ptr = reinterpret_cast<byte*> (ui2_ptr);
                        break;
                    }
                    case 2: {
                        if (length < 4) // 4 byte for the width word.
                            return ErrorReport (bin, kErrorBufferUnderflow,
                                              params, index, start);
                        length -= 4;
                        ui4_ptr = reinterpret_cast<uint32_t*> (args[arg_index]);
                        if (ui4_ptr == nullptr)
                            return ErrorReport (bin, kErrorImplementation, params,
                                              index, start);

                        for (temp = 0; temp <= sizeof (uint32_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui4 |= ((uint32_t)ui1) << temp;
                        }
                        if (ui4 >= length)
                            return ErrorReport (bin, kErrorBufferOverflow,
                                              params, index, start);
                        length -= count;
                        count = (uintptr_t)ui4;
                        ui1_ptr = reinterpret_cast<byte*> (ui4_ptr);
                        break;
                    }
                    case 3: { // 8 byte for the width word.
                        if (length < 9)
                            return ErrorReport (bin, kErrorBufferUnderflow,
                                              params, index, start);
                        length -= 8;
                        ui8_ptr = reinterpret_cast<uint64_t*> (args[arg_index]);
                        if (ui8_ptr == nullptr)
                            return ErrorReport (bin, kErrorImplementation, params,
                                              index, start);

                        for (temp = 0; temp <= sizeof (uint64_t); temp += 8) {
                            ui1 = *start;
                            if (++start >= end) start -= size;
                            hash = Hash16 (ui1, hash);
                            ui8 |= ((uint64_t)ui1) << temp;
                        }
                        if (ui8 > length)
                            return ErrorReport (bin, kErrorBufferOverflow,
                                              params, index, start);
                        length -= count;
                        count = (uintptr_t)ui8;
                        ui1_ptr = reinterpret_cast<byte*> (ui8_ptr);
                        break;

                    }
                    default: return ErrorReport (bin, kErrorImplementation, params,
                                               index, start);
                }

                if (length < count)
                    return ErrorReport (bin, kErrorBufferOverflow, params,
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
    printf ("\n| Hash expected: %x ", hash);
#endif
    if (length < 2)
        return ErrorReport (bin, kErrorBufferUnderflow, params, index,
                          start);
    ui2 = *start;
    if (++start >= end) start -= size;
    ui1 = *start;
    if (++start >= end) start -= size;
    ui2 |= (((uint16_t)ui1) << 8);
#if SCRIPT_DEBUG
    printf ("found: %x\n", ui2);
#endif
    if (hash != ui2)
        return ErrorReport (bin, kErrorInvalidHash, params, index,
                          start);

//#if SCRIPT_DEBUG
//    //printf ("\n| Done reading\n");
//    SlotClear (begin, bin->start, start, stop, end, size);
//#endif

    // Convert pointer back to offset
    bin->start = (uint_t)Diff (begin, start);

    return 0;
}

#if SCRIPT_USING_TEXT
Text& BinPrint (Bin* bin, Text& text) {
    if (!bin) {
        return text << "\n| Error: Bin can't be null";
    }
    uint_t size = bin->size;
    return text << text.Line ('_')
               << "\n| Bin:" << text.Pointer (bin) << " size:" << bin->size
               << " start:"  << bin->start        << " stop:" << bin->stop
               << " read:"   << bin->read
               << text.Memory (BinBuffer (bin), size + sizeof (Bin));
}
#endif  //< SCRIPT_USING_TEXT

}       //< namespace _
