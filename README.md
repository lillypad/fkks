# Finders Keepers (KPot Stealers)

![anime credit card](docs/img/anime-credit-card.gif)

- Samples in the file `samples.zip` the password is `infected`.
- Code for decoding the strings for the stage 2 loader are in the `src` directory
- Slides are located in `docs/index.pdf`

# KPot v2.0 Crypt/Decrypt

By turning KPot v2.0 against itself with pseudo emulation techniques, I was able to decrypt all of its strings and its C2 exfiltration data.

:notebook: Yes, there are two string decryption functions, they are however redundant. :laughing:

__Building and Using the Crypt/Decrypt Tool:__
```bash
malware@work ~$ cd src/kp2dc/
malware@work ~$ make
malware@work ~$ make extract-sample
malware@work ~$ bin/kp2dc --help
/----------------------------------------------------------\
|              KPot v2.0 Decrypt/Encrypt                   |
|----------------------------------------------------------|
|  -h, --help                Print this Help Menu          |
|  --decrypt-strings         Dump All Strings              |
|  -q, --quiet               Silence Output                |
|  --extract-key             Dump C2 Server Decryption Key |
|  --extract-domain          Dump C2 Domain                |
|  --decrypt-sid      [#]    Decrypt String by ID          |
|  -k, --key        [keystr] Crypt Key String              |
|  --crypt-file              Crypt File                    |
|  -o, --output     [_file_] Output to File                |
|  -i, --input      [_file_] Input File                    |
|  --crypt-stdin             StdIn (base64 only)           |
|  --examples                Show Examples                 |
|  -v, --version             Show Version                  |
|----------------------------------------------------------|
| Company: GoSecure TITAN                                  |
| Author : Lilly Chalupowski                               |
\----------------------------------------------------------/
malware@work ~$ bin/kp2dc --examples
kp2dc --extract-key -i sample.bin
kp2dc --extract-domain -i sample.bin
kp2dc --decrypt-strings -i sample.bin
kp2dc --examples
kp2dc --crypt-file -i data.bin --key [KEY] -o out.bin
cat c2.b64 | kp2dc --crypt-stdin --key [KEY] -o out.bin
kp2dc --decrypt-strings --sid-low 0 --sid-high 182 -i sample.bin
kp2dc --decrypt-sid [SID#] -i sample.bin
malware@work ~$ make install
```

The `src/kp2dc/src/c2/server.py` script can be used to raise your own C2 server, it will write `bin` files to the same folder with the date-time. You can decrypt these files later on using the `kp2dc` tool that you extracted the key with using the `--crypt-file` method. :laughing:

KPot v2.0 uses IDs from `0` to `182` and other hard-coded data to perform the XOR decryption.

If you like this research and want to support my work give me a :star:.

If you have questions send me a message! :smile_cat:

Thank you! \\^-^/
