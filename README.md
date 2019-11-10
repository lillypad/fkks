# Finders Keepers (KPot Stealers)

![anime credit card](docs/img/anime-credit-card.gif)

- Samples in the file `samples.zip` the password is `infected`.
- Code for decoding the strings for the stage 2 loader are in the `src` directory
- Slides are located in `docs/index.pdf`

# KPot v2.0 Decryptor

By turning KPot v2.0 against itself with pseudo emulation techniques, I was able to decrypt all of its strings.

:notebook: Yes, there are two string decryption functions, they are however redundant. :laughing:

```bash
cd src/kp2dc/
make                             # Build KPot v2.0 Encrypt/Decrypt Tool
make extract-sample              # Extract KPot v2.0 Sample (password: 'infected')
cd bin/
./kp2dc -d sample.bin            # C2 Domain Decryption
./kp2dc -k sample.bin            # C2 Communication Key Decryption
./kp2dc -a sample.bin            # Decrypt All Strings
./kp2dc -c [base64]              # Decrypt C2 Data (incomplete)
cat c2.b64 | ./kp2dc --std-in -c # Decrypt C2 Data (incomplete)
```

KPot v2.0 uses IDs from `0` to `182` and other hard-coded data to perform the XOR decryption.

I will create a KPot v2.0 C2 server decryptor as well, though string decryption should help other researchers for now. :smirk_cat:

If you like this research and want to support my work give me a :star:.

If you have questions send me a message! :smile_cat:

Thank you! \\^-^/
