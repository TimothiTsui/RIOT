1. Install latest python for windows: https://www.python.org/downloads/windows/
2. In cmd.exe and type: python -m pip install pyreadline (readline not compatible with windows)
3. python pip install twisted -> causes exception like this:

	Traceback (most recent call last):
  File "C:\Users\skull\AppData\Local\Programs\Python\Python36-32\lib\site-packages\pip\compat\__init__.py", line 73, in console_to_str
    return s.decode(sys.__stdout__.encoding)
UnicodeDecodeError: 'utf-8' codec can't decode byte 0x81 in position 118: invalid start byte

During handling of the above exception, another exception occurred:

Traceback (most recent call last):
  File "C:\Users\skull\AppData\Local\Programs\Python\Python36-32\lib\site-packages\pip\basecommand.py", line 215, in main
    status = self.run(options, args)
  File "C:\Users\skull\AppData\Local\Programs\Python\Python36-32\lib\site-packages\pip\commands\install.py", line 342, in run
    prefix=options.prefix_path,
  File "C:\Users\skull\AppData\Local\Programs\Python\Python36-32\lib\site-packages\pip\req\req_set.py", line 784, in install
    **kwargs
  File "C:\Users\skull\AppData\Local\Programs\Python\Python36-32\lib\site-packages\pip\req\req_install.py", line 878, in install
    spinner=spinner,
  File "C:\Users\skull\AppData\Local\Programs\Python\Python36-32\lib\site-packages\pip\utils\__init__.py", line 676, in call_subprocess
    line = console_to_str(proc.stdout.readline())
  File "C:\Users\skull\AppData\Local\Programs\Python\Python36-32\lib\site-packages\pip\compat\__init__.py", line 75, in console_to_str
    return s.decode('utf_8')
UnicodeDecodeError: 'utf-8' codec can't decode byte 0x81 in position 118: invalid start byte

%C:\Users\skull\AppData\Local\Programs\Python\Python36-32\Lib\site-packages\pip\compat

4. Solution: run cmd.exe and type: chcp
4.1 It will show the system default code, for example 936
4.2 Open the file in editor: C:\Users\User\AppData\Local\Programs\Python\Python36-32\Lib\site-packages\pip\compat\__init__.py
4.3 Arround line 75: change "return s.decode('utf_8')" to "return s.decode('cp936')" (instead of cp936 enter the code you got from chcp



Other changes:
1. In file pyterm: Line 27, Line 31-35 (pyreadline instead of readline)

2. in file pyterm: Line 60 works only in linux. 

Previous: defaultdir      = os.environ['HOME'] + os.path.sep + '.pyterm' 

Changed to: 

import os.path
home_folder = os.path.expanduser('~')

# directory to store configuration and log files
defaultdir      = home_folder + os.path.sep + '.pyterm'