/**
 * Copyright 2019 AbbeyCatUK
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdint.h>


#ifndef CPU_STATE_H
#define CPU_STATE_H


    /*
     * reserves space for 17 x 32-bit registers:
     *   - r0-r12
     *   - sp, lr
     *   - cpsr, pc
     */
    typedef struct cpu_state {
        uintptr_t r[ 32 ];
    } cpu_state;


    int     _kernel_process_init_cpu_state( cpu_state *state );


#endif /*CPU_STATE_H*/