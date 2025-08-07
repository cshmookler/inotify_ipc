/*****************************************************************************/
/*  Copyright (c) 2025 Caden Shmookler                                       */
/*                                                                           */
/*  This software is provided 'as-is', without any express or implied        */
/*  warranty. In no event will the authors be held liable for any damages    */
/*  arising from the use of this software.                                   */
/*                                                                           */
/*  Permission is granted to anyone to use this software for any purpose,    */
/*  including commercial applications, and to alter it and redistribute it   */
/*  freely, subject to the following restrictions:                           */
/*                                                                           */
/*  1. The origin of this software must not be misrepresented; you must not  */
/*     claim that you wrote the original software. If you use this software  */
/*     in a product, an acknowledgment in the product documentation would    */
/*     be appreciated but is not required.                                   */
/*  2. Altered source versions must be plainly marked as such, and must not  */
/*     be misrepresented as being the original software.                     */
/*  3. This notice may not be removed or altered from any source             */
/*     distribution.                                                         */
/*****************************************************************************/

#ifndef IIPC_IIPC_H
#define IIPC_IIPC_H

#ifdef __cplusplus
    #include "iipc.hpp"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void iipc_string_free(char* error);

#ifdef __cplusplus
using iipc_channel_t = iipc::channel_t;
#else
typedef struct iipc_channel_t iipc_channel_t;
#endif
iipc_channel_t* iipc_get_channel(const char* path, char** error);
void iipc_channel_free(iipc_channel_t* channel);
char* iipc_channel_get_path(iipc_channel_t* channel, char** error);
void iipc_channel_set_path(
  iipc_channel_t* channel, const char* path, char** error);
void iipc_channel_send(
  iipc_channel_t* channel, const char* message, char** error);
int iipc_channel_poll(
  iipc_channel_t* channel, unsigned long timeout_ms, char** error);
char* iipc_channel_receive(iipc_channel_t* channel, char** error);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // IIPC_IIPC_H
