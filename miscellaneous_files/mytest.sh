#!/bin/bash

# Test Built-in commands
echo "Testing Built-in Commands"

echo "Running: echo Hello"
echo Hello  # Expected output: Hello

echo "Running: pwd"
pwd  # Expected output: /current/directory

echo "Running: cd /home/user"
cd /home/user
pwd  # Expected output: /home/user

echo "Running: exit"
exit  # Should terminate the shell script

# Test External Commands via Absolute Path
echo "Testing External Command via Absolute Path"

echo "Running: /bin/ls"
/bin/ls  # Expected output: List of files in root directory

echo "Running: /usr/bin/python3 --version"
/usr/bin/python3 --version  # Expected output: Python version

# Test External Command via Relative Path
echo "Testing External Command via Relative Path"

echo "Running: ./my_script.sh"
chmod +x ./my_script.sh  # Ensure it's executable
./my_script.sh  # Expected output from the script

# Edge Case: Command not found
echo "Testing Edge Case: Non-existing command"
non_existing_command  # Expected output: command not found
