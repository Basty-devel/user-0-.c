/*
 * user[0].c
 * 
 * Copyright 2024  <Sebastian.Nestler@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <crypt.h>

int main() {
    // Init of the variables password, salt and hashed password
    char password[100];
    char salt[] = "$6$rounds=9876$";
    char hashed_password[100];

    // Input new password
    printf("Enter the password to be hashed: ");
    scanf("%s", password);

    // Copy of the result of crypt() into hashed_password
    strcpy(hashed_password, crypt(password, salt));

    // Print hash to screen
    printf("Hashed password: %s\n", hashed_password);

    // Test if /etc/shadow is readable
    FILE *shadow = fopen("/etc/shadow", "r+");
    if (shadow == NULL) 
    {
        printf("Error opening /etc/shadow\n");
        return 1;
    }

    char line[100];
    // Getting /etc/shadow
    while (fgets(line, sizeof(line), shadow)) 
    {
        // Searching root line
        if (strncmp(line, "root:", 5) == 0) 
        {
            /**
            * This code snippet searches for a specific line in a string and extracts a substring between two characters.
            * It looks for the first occurrence of the character '$' and the character ':' in the given line.
            * If both characters are found, it performs the following operations.
            * 
            * - Assigns the pointer to the first occurrence of '$' to the variable hash_start.
            * - Assigns the pointer to the first occurrence of ':' to the variable hash_end.
            * 
            * The code snippet checks if both hash_start and hash_end are not NULL before performing the operations.
            * 
            * Note: This code snippet assumes that the line is a null-terminated string.
            */
            char *hash_start = strchr(line, '$');
            char *hash_end = strchr(line, ':');
            if (hash_start && hash_end) 
            {
                /** This code snippet is a part of a while loop that searches for a specific line in the /etc/shadow file.
                * It calculates the length of the hash substring in the line and checks if it is within the bounds of the hashed_password array.
                * If the length is valid, it copies the hashed_password into the line at the appropriate position.
                * The break statement is used to exit the loop after the operation is performed.*/
                int hash_len = hash_end - hash_start;
                if (hash_len > 0 && hash_len < sizeof(hashed_password)) 
                {
                    strncpy(line + hash_start - line, hashed_password, hash_len);
                    break;
                }
            }
        }
    }
    // Resets the file pointer and truncates the content of the file to 0 bytes
    fseek(shadow, 0, SEEK_SET);
    ftruncate(fileno(shadow), 0);
    rewind(shadow);

    fputs(line, shadow);
    fclose(shadow);

    return 0;
}
