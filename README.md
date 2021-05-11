<b>Problem Statement:</b><br />
An array consists of 1,000,000 elements each initialized from 0 to 999,999 respectively. Using threads defined by user input increment the value of each element in the array by 5 and  report the execution time for different number of cores and different number of threads for each core.

<b>Program Execution Instructions un Ubuntu 20.04</b>
- Validate if gcc is installed using terminal command: ```gcc --version```
- Save the code and open in IDE of choice.
- Build and compile using IDE or terminal.
- To compile and run using Visual Studio Code:
  - Automated Process:
    - Press the shortcut keys: ```Ctrl+B``` or ```Ctrl+Shift+B``` to open the ```tasks.json``` file (Only done once).
    - Under ```"args": [...]``` add the following elements ```"-g","-pthread"```
    - Change ```"group": "kind": "build",``` to ```"group": {"isDefault": true, "kind": "build"},```
    - Press ```Ctrl+Shift+B``` to build
    - Open terminal and type ```./ass_1```
  - Alternatively, open the terminal in Visual Studio and follow the steps below.
- To compile and run using terminal (VS Terminal or Ubuntu Terminal):
  - Navigate terminal to the directory where 'ass_1.c' is stored.
  - Terminal Command to compile: ```gcc ass_1.c -o outputFileName -pthread```
  - Terminal Command to execute: ```./outputFileName```

<b>Created and Developed by:</b>
- Keshav Mathur (5773878)
- Sania Shehzad (5981384)

<b>References:</b>
1. To get the number of cores: https://stackoverflow.com/questions/4586405/how-to-get-the-number-of-cpus-in-linux-using-c (Author: Владимир Николайчук)
2. To clear screen: https://www.includehelp.com/c-programming-questions/how-to-clear-output-screen-in-c.aspx
3. To check if file is empty: https://stackoverflow.com/questions/13566082/how-to-check-if-a-file-has-content-or-not-using-c
4. To use a micro symbol: https://askubuntu.com/questions/1098090/how-do-i-type-a-%C2%B5-micro-symbol
