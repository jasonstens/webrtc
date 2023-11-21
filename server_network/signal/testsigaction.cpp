 #include <iostream>
 #include <signal.h>
 #include <unistd.h>

void sighandler(int sig) {
    std::cout << "recieved sig is " << sig << std::endl;

}

 int main()
 {
    struct sigaction act, oact;
    act.sa_handler = sighandler;
    // 在处理此信号时屏蔽其他信号
    sigfillset(&act.sa_mask);
    // 决定使用不带信号信息的处理函数
    act.sa_flags = 0;

    sigaction(SIGINT, &act, &oact);
    pause();

    return 0;
 }