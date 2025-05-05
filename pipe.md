# what needed to implement pipe
- parser able to parse pipe symbol
- add pipe to enum of separator
- executioner.exec shouldnt wait, but instead push the child pid to container
- implement executioner.wait and waitAll
    - wait(pid)
    - waitAll(), get all pid from container
- pipefd has to be store somewhere, so we can close it easily
    - by looping thru the container and close if that fd is not the same as givenfd (get from param)
- exectree should get pipefd
    - which cause executioner.exec to also have to handle the fd

# pseudo code 
execTree(Node, writeFd, readFd) {
    if Node.separator == pipe {
        int pipefd[2];
        pipe(pipefd);
        push pipefd to container
        run = execTree(left, pipefd[1],null);
        if !run return run; // if exit then imedietly exit (TODO:when close pipe?)
        run = execTree(left, null,pipefd[2]);
        if !run return run;
        closePipe()
    }
    
}
executioner::exec(args, writeFd,readFd) {
    fork
    if child {
        dup fd
        closePipe(containerPipe thats not (writefd or readfd))
    }
}
parser.getCommands(input) {
    for char in input
        separator = S_NONE
        if char == | {
            separator = pipe
        } 
        if char == | && nextChar == | {
            separator = S_OR
        } 
}
executioner::wait(pid) {
    // take the parent part of exec, put it here
}
executioner::waitAll() {
    for pid in containerPid
        // dont forget to only return exitcode of last command
        executioner::wait(pid)
}
