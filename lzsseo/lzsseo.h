/*
Copyright (c) 2016, Conor Stokes
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LZSSEO_H__
#define LZSSEO_H__

#pragma once

typedef struct LZSSEO_OptimalParseState LZSSEO_OptimalParseState;

    // Make a compressor state for the re-entrant version of 
LZSSEO_OptimalParseState* LZSSEO_MakeOptimalParseState( size_t bufferSize );

void LZSSEO_FreeOptimalParseState( LZSSEO_OptimalParseState* toFree );

size_t LZSSEO_CompressOptimalParse( LZSSEO_OptimalParseState* state, const char* input, size_t inputLength, char* output, size_t outputLength, unsigned int level );

size_t LZSSEO_Decompress( const char* input, size_t inputLength, char* output, size_t outputLength );

#endif // -- LZSSEO_H__