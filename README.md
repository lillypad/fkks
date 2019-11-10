# Finders Keepers (KPot Stealers)

![anime credit card](docs/img/anime-credit-card.gif)

- Samples in the file `samples.zip` the password is `infected`.
- Code for decoding the strings for the stage 2 loader are in the `src` directory
- Slides are located in `docs/index.pdf`

# KPot v2.0 String Decryptor

By turning KPot v2.0 against itself with pseudo emulation techniques I was able to decrypt most of it's strings.

```bash
cd src/kp2dc/
make
make extract-sample
cd bin/
./kp2dc -d sample.bin # C2 Domain Decryption
./kp2dc -k sample.bin # C2 Communication Key Decryption
./kp2dc -a sample.bin # Decrypt All Strings
```

KPot v2.0 uses IDs from `0` to `182` and other hard-coded data to perform the XOR decryption.

I will create a KPot v2.0 C2 server decryptor as well, though string decryption should help other researchers for now. :smirk_cat:

If you like this research and want to support my work give me a :star:.

If you have questions send me a message! :smile_cat:

Thank you! \\^-^/
