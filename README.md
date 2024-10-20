# hashcheck

**Hashcheck** is a specialized password-cracking tool that automates the identification of hash types before attempting to crack passwords using **hashcat**. It is designed for efficiency, ensuring that the correct hash argument is selected and passed directly to hashcat, streamlining the password recovery process. By handling hash identification and argument setting in a seamless manner, Hashcheck minimizes manual intervention, making it a versatile asset for penetration testers and security professionals.

The core functionality of Hashcheck revolves around its ability to **detect the hash type automatically**. Once a hash is inputted, the tool runs it through a series of checks to determine whether it’s MD5, SHA-1, SHA-256, or other common hash formats. Based on the detected type, Hashcheck selects the appropriate hashcat argument, ensuring compatibility and accuracy for the cracking phase. This intelligent detection system optimizes hashcat’s operations, allowing it to run more efficiently and effectively.

Customization is a key feature of Hashcheck, especially in its ability to **configure wordlists**. Users can set the desired wordlist from a JSON configuration file, offering flexibility to adapt to different cracking scenarios or environments. This feature makes it adaptable for various use cases, whether targeting weak passwords quickly or performing more extensive cracking attempts.

In addition to its primary functionalities, Hashcheck is built with extensibility in mind. Its modular design allows users to add support for new hash types or modify existing configurations as needed. This flexibility, combined with its integration with hashcat, makes it an essential tool for anyone involved in password security testing or ethical hacking, ensuring both speed and precision in the password-cracking workflow.

## How the program works

1. Reads the input hash from the command line, file, or user input.
2. Determines the hash type using the HashDetector class.
    * Uses length and regex patterns to identify the hash type and the corresponding hashcat mode.
3. Loads the configuration from the config.json file using the FileHandler class.
4. Generates the hashcat command using the detected hash type, hashcat mode, and configuration parameters.
5. Executes the command using the HashcatExecutor class, which calls the system() function to run hashcat.

## Examples of using hashcheck

### Using command line input
```
./bin/hash_detector -i e99a18c428cb38d5f260853678922e03 -c config.json
```

### Using File Input

If you have a file hash.txt containing a SHA256 hash, you can run:

```
./bin/hash_detector -f hash.txt -c config.json
```
