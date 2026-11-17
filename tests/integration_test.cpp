#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>
#ifndef _WIN32
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#else
#include <process.h>
#endif
namespace fs = std::filesystem;
static void require(bool ok, const std::string& what) { if (!ok) throw std::runtime_error(what); }
static void write(const fs::path& p, const std::string& data) { std::ofstream o(p, std::ios::binary); require(bool(o), "write " + p.string()); o << data; }
static std::string read(const fs::path& p) { std::ifstream in(p, std::ios::binary); return {std::istreambuf_iterator<char>(in), {}}; }
static std::string jsonEscape(const std::string& s) { std::string out; for(char c:s) { if(c=='\\'||c=='"')out+='\\'; out+=c; } return out; }
static std::string pack(const std::vector<std::string>& v) { std::string s; for(auto& a:v) s+=std::to_string(a.size())+":"+a; return s; }
struct Result { int status; std::string output; };
static Result run(const fs::path& exe, const std::vector<std::string>& args) {
#ifndef _WIN32
    int pipefd[2]; require(pipe(pipefd)==0,"pipe");
    pid_t pid=fork(); require(pid>=0,"fork");
    if(pid==0) {
        close(pipefd[0]); dup2(pipefd[1],STDOUT_FILENO); dup2(pipefd[1],STDERR_FILENO); close(pipefd[1]);
        std::vector<std::string> all{exe.string()}; all.insert(all.end(),args.begin(),args.end());
        std::vector<char*> argv; for(auto& s:all)argv.push_back(s.data()); argv.push_back(nullptr);
        execv(argv[0],argv.data()); _exit(127);
    }
    close(pipefd[1]); std::string output; char buf[4096]; ssize_t n;
    while((n=::read(pipefd[0],buf,sizeof(buf)))>0) output.append(buf,static_cast<size_t>(n));
    close(pipefd[0]); int status=0; require(waitpid(pid,&status,0)==pid,"waitpid");
    return {WIFEXITED(status)?WEXITSTATUS(status):128,output};
#else
    std::vector<std::string> all{exe.string()}; all.insert(all.end(),args.begin(),args.end());
    std::vector<const char*> argv; for(auto& s:all)argv.push_back(s.c_str()); argv.push_back(nullptr);
    int code=_spawnv(_P_WAIT,argv[0],argv.data()); return {code,""};
#endif
}
static void envset(const char* key,const std::string& val) {
#ifdef _WIN32
    _putenv_s(key,val.c_str());
#else
    setenv(key,val.c_str(),1);
#endif
}
int main(int argc,char** argv) {
    try {
        require(argc==3,"usage: integration_test <hashcheck> <mock_hashcat>");
        const fs::path app=fs::absolute(argv[1]), mock=fs::absolute(argv[2]);
        const fs::path root=fs::temp_directory_path()/("hashcheck cpp regression "+std::to_string(
#ifndef _WIN32
            getpid()
#else
            _getpid()
#endif
        ));
        fs::create_directories(root);
        struct Cleanup {fs::path path; ~Cleanup(){std::error_code ec;fs::remove_all(path,ec);}} cleanup{root};
        const auto log=root/"argv.log", word=root/"word list;$(echo injected).txt", config=root/"config.json";
        write(word,"test\n"); envset("HASHCHECK_TEST_LOG",log.string()); envset("HASHCHECK_TEST_EXIT","0");
        auto configure=[&](const std::string& w){write(config,"{\"hashcat_path\":\""+jsonEscape(mock.string())+"\",\"wordlist\":\""+jsonEscape(w)+"\"}");};
        configure(word.string());
        const std::string h(32,'a');
        auto cli=[&](std::vector<std::string> a){return run(app,a);};
        auto base=[&](){return std::vector<std::string>{"-i",h,"-c",config.string(),"--mode","0"};};
        int count=0;
        auto check=[&](bool ok,const std::string& name){require(ok,name); ++count; std::cout<<"PASS "<<name<<'\n';};
        auto r=cli(base());
        check(r.status==0 && read(log)==pack({"-m","0","-a","0",h,word.string()}),"literal argv including special wordlist path");
        fs::remove(log);
        r=cli({"-i","$2b$12$"+std::string(53,'a'),"-c",config.string()});
        check(r.status==0 && read(log)==pack({"-m","3200","-a","0","$2b$12$"+std::string(53,'a'),word.string()}),"structured bcrypt mode");
        fs::remove(log);
        r=cli({"-i",std::string(64,'a'),"-c",config.string()});
        check(r.status!=0 && !fs::exists(log),"ambiguous hash rejected");
        auto dry=base();dry.push_back("--dry-run");r=cli(dry);
        check(r.status==0 && !fs::exists(log),"dry run does not execute");
        auto input=root/"input hash.txt";write(input,h+"\n");
        r=cli({"-f",input.string(),"-c",config.string(),"--mode","0"});
        check(r.status==0,"file input");fs::remove(log);
        write(config,"{broken");r=cli(base());
        check(r.status!=0 && !fs::exists(log),"invalid JSON");configure(word.string());
        r=cli({"-f",(root/"missing").string(),"-c",config.string(),"--mode","0"});
        check(r.status!=0,"missing hash file");
        for(auto bad: {std::string("[]"),std::string("{}"),std::string("{\"hashcat_path\":123,\"wordlist\":\"x\"}")}) {
            write(config,bad);r=cli(base());check(r.status!=0,"invalid config shape");
        }
        configure(word.string());
        for(auto args: {std::vector<std::string>{"--bad"},{"-i"},{"--mode","nope"},{"-i","a","-f","b"}}) {
            r=cli(args);check(r.status!=0,"invalid CLI arguments");
        }
        write(input,h+"\n"+h+"\n");r=cli({"-f",input.string(),"-c",config.string(),"--mode","0"});
        check(r.status!=0,"multiple hashes rejected");
        fs::remove(word);r=cli(base());check(r.status!=0,"missing wordlist rejected");write(word,"test\n");
        auto duplicate=base();duplicate.insert(duplicate.end(),{"--mode","0"});r=cli(duplicate);
        check(r.status!=0,"duplicate mode rejected");
        for(int status: {1,2,42}) {envset("HASHCHECK_TEST_EXIT",std::to_string(status));r=cli(base());check(r.status!=0,"Hashcat failure status "+std::to_string(status));}
        envset("HASHCHECK_TEST_EXIT","0");
        const auto marker=root/"INJECTION_MARKER"; const auto evil="$(touch "+marker.string()+")";
        r=cli({"-i",evil,"-c",config.string(),"--mode","0"});
        check(r.status==0 && !fs::exists(marker) && read(log)==pack({"-m","0","-a","0",evil,word.string()}),"no shell interpretation");
        std::cout<<"Passed "<<count<<" C++ integration checks\n";return 0;
    } catch(const std::exception& e) {std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}
}
