关于Robbe: 

robbe是建立在friso上的php中文分词扩展，请到http://code.google.com/p/friso去下载最新版本的friso。
(直接使用里面的二进制共享文件，或者自己编译安装friso)

1. 如何安装:
(1). 安装friso
->Linux: 请查看friso的README.txt文档
->WinNT: 请去friso官方网站下载最新的friso.dll, 将friso.dll放在C:/windows或者C:/windows/system32或者php的ext目录下.
++++++++++++++++++++++++++
并且需要将friso里面的dict和friso.ini拷贝到一个好点的目录吧, 下面的robbe需要friso.ini配置文件和dict里面的所有词库文件.
并且你还需要配置friso.ini中的friso.lex_dir选项的值为 friso词库目录, 即dict目录的绝对路径, 并且这个值必须以"/"结束

(2).安装robbe
->Linux: 直接编译. 如果觉得麻烦, 可以直接使用robbe/lib/linux/robbe.so
(编译注意事项: http://www.oschina.net/question/853816_119031)
在php配置文件中加入: extension=robbe.so

->WinNT: 动态链接库文件: robbe/lib/win32/robbe.dll
在php配置文件中加入: extension=php_robbe.dll  (将php_robbe-vc6.dll重命名下)

(3).配置robbe: 在php.ini配置文件中加入如下内容:
[robbe]
;configuration file for robbe.
robbe.ini_file = friso.ini文件的绝对地址(在下载下来的friso附件中)

作者：狮子的魂 - 陈鑫
电子邮件：chenxin619315@gmail.com
robbe详细介绍：http://code.google.com/p/robbe
https://code.google.com/p/robbe/#三。使用方法
