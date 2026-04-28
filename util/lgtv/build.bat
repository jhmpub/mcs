rem build lgtv console and gui executables stored in ./dist directory  
rem 
rem pyinstaller package is required 
rem https://pyinstaller.org/
rem 
rem python -m pip install pyinstaller
rem 

pyinstaller --clean --onefile --noconfirm --paths=.\module --noconsole --name lgtv_gui lgtv.py
pyinstaller --clean --onefile --noconfirm --paths=.\module lgtv.py
