# Automatically abort script on errors
option batch abort
# Disable overwrite confirmations that conflict w/the previous
option confirm off
option batch on
# Connect to kauailabs ftp server using a password
open ftp://%FTP_USERNAME%:%FTP_PASSWORD%@ftp.kauailabs.com/
# Synchronize files
put -neweronly -transfer=binary ..\navx-micro.zip kauailabs.com/public_files/navx-micro/
put -neweronly -transfer=binary ..\navx-micro-libs.zip kauailabs.com/public_files/navx-micro/
cd kauailabs.com/public_files/navx-micro
mkdir installers
synchronize remote -transfer=binary ../installers installers/
mkdir drivers
synchronize remote -transfer=binary ../drivers drivers/
# Disconnect
close
# Quit
exit