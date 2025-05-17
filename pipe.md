# what needed to implement pipe
- [x] parser able to parse pipe symbol
- [x] add pipe to enum of separator
    - we make lexer
- [ ] NEXT: executioner.exec shouldnt wait, but instead push the child pid to container
- implement executioner.wait and waitAll
    - wait(pid)
    - waitAll(), get all pid from container
- pipefd has to be store somewhere, so we can close it easily
    - by looping thru the container and close if that fd is not the same as givenfd (get from param)
- we will implement pipe using loop method

# pseudo code 
execpipe
init all the pipe needed
push all the pipefd to vec to make it easier to work with
exec every children with pipe and push the pid
wait last pid and use that exitcode
wait the rest

