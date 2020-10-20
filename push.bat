::删除编译生成的文件
::rd /s /q MysqlOrm\.vs
::rd /s /q MysqlOrm\x64
::rd /s /q MysqlOrm\MysqlOrm\x64


::上传到git
git add .
git commit -m "modify 数据库池使用示例.txt"
git push origin master
echo "push complete"
pause