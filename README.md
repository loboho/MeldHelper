# MeldHelper
use Meld to compare two files/folders via shell menu in microsoft windows
# How to use
* install meld for windows from https://meld.app/
* assume that you have already installed Meld in C:\Program Files\Meld
* build MeldHelper
* copy MeldHelper.exe to C:\Program Files\Meld
* install shell context menu
    * run command prompt as administrator, type the following commands:
```
REG ADD "hkcr\Folder\shell\Meld Compare" /t REG_SZ /v icon /d "C:\Program Files\Meld\Meld.exe"
REG ADD "hkcr\Folder\shell\Meld Compare\command" /t REG_SZ /d "C:\Program Files\Meld\MeldHelper.exe %1"
REG ADD "hkcr\*\shell\Meld Compare" /t REG_SZ /v icon /d "C:\Program Files\Meld\Meld.exe"
REG ADD "hkcr\*\shell\Meld Compare\command" /t REG_SZ /d "C:\Program Files\Meld\MeldHelper.exe %1"
```
* right click the first target (file/folder) in explorer and select "Meld Compare"
* right click the second target (file/folder) and select "Meld Compare", now the Meld Comparation will start
* Note：Once you have choosed the first target, you should choose the second target within 3 minutes, otherwise the Comparation will be canceled
