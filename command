mkdir file 创建一个文件夹
cd file 进入file文件
git init 将该文件创建为git仓库
git config --global user.name "****" 配置用户名
git config --global email.name "****" 配置邮箱
git config --global credential.helper store 保存上面配置
git config --global --list 查看配置信息
ll 查看工作区文件
git ls-files 查看暂存区文件
git status 查看文件提交状态，被修改未暂存，已暂存未提交
git add 将工作区文件提交到暂存区
git commit -m "****" 将暂存区文件提交到本地仓库，"****"对该次提交的描述
git commit -a -m "****" -a参数将上述两个命令合并为一个，暂存和提交一起执行
git diff [HEAD,--cached] 不加参数git diff查看工作区，暂存区之间的差异，HEAD是指向本地仓库唯一master分支的指针，查看本地仓库和工作区，暂存区之间的差异，--cached查看暂存区和本地仓库之间的差异
git log [--oneline] 查看提交历史
git reset [--soft,--hard,--mixed] 回退到上一次提交，--soft工作区和暂存区中的内容都不删除，--hard将工作区和暂存区中的内容都删除，--mixed删除暂存区，保留工作区中的内容
git rm file 删除工作区和暂存区中的文件file，重新提交后相当于删除本地仓库中的文件file
ssh-keygen -t rsa -C "github邮箱" 需在C:\Users\用户文件下创建.ssh文件夹，使用前面命令生成ssh私钥和公钥，默认是id_rsa文件，如果不是第一次配置，需要自己定义文件名以防止原来的ssh密钥被覆盖
ssh -T git@github.com 用来查看是否成功连接github仓库
git remote add origin git@github.com:yansitiaoyu/git-learn.git 建立本地仓库和远程仓库的连接
git branch -M main 更改本地仓库主分支的名称，改为main使其远程仓库主分支名称相同
git push -u origin mian 使本地主分支与远程主分支关联
git clone git@github.com:yansitiaoyu/git-learn.git 是将远程仓库克隆到本地
git branch branch-name 以当前main分支为基础创建名为branch-name的分支
git branch 查看当前被创建的分支以及处在哪个分支上
git switch branch-name 切换到branch-name分支上
git merge branch-name 将branch-name分支合并到当前分支上
git branch -d branch-name 将已经合并的branch-name分支删除，未合并的分支这条命令不会执行，强制删除参数是-D
git merge --abort 终止合并命令
git rebase branch-name 变基合并命令，branch-name是合并后的基底
"gcc hello.c -o hello" 命令编译生成可执行文件hello
"gcc -E hello.c -o hello.i" 命令将源文件和相关头文件预编译成.i文件
"gcc -S hello.i -o hello.s" 命令将预处理文件预编译成.s汇编代码文件 
"gcc -c hello.s -o hello.o" 命令将汇编文件预编译成"中间.o目标代码文件 
"gcc hello.o -o hello" 命令生成可执行文件 
vsplit 文件路径 是将命令行窗口垂直打开另外一个窗口
