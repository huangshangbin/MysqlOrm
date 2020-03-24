#删除编译生成的文件
rd /s /q MysqlOrm\x64
rd /s /q MysqlOrm\MysqlOrm\x64
git add .
git commit -m "script push"
git push origin master
echo "push complete"
pause