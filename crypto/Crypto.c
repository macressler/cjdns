/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "crypto/Crypto.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"

#include "util/Assert.h"
#include "util/Base32.h"
#include <stdio.h>
#include <event2/util.h>


/** @see Crypto.h */
void Crypto_init()
{
    if (evutil_secure_rng_init() != 0) {
        fprintf(stderr, "Unable to initialize secure random number generator, bailing out.");
        abort();
    }
}

/**
 * This is used by libnacl which requires us to provide it with a randombytes() function.
 */
void randombytes(unsigned char* buffer,unsigned long long size) // CHECKFILES_IGNORE
{
    evutil_secure_rng_get_bytes(buffer, size);
}

void Crypto_randomBase32(uint8_t* output, uint32_t length)
{
    uint32_t index = 0;
    for (;;) {
        uint8_t bin[16];
        randombytes(bin, 16);
        int ret = Base32_encode(&output[index], length - index, bin, 16);
        if (ret == Base32_TOO_BIG || index + ret == length) {
            break;
        }
        index += ret;
    }
}
