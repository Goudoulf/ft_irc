#pragma once
#include "Command.hpp"
#include "ParamTemplate.hpp"
#include "cmds.h"
#include "reply.h"
#include <string>
#include <utility>
#include "CmdLevel.h"

class Command;
class ParamTemplate;

class TemplateBuilder {

public:
  ~TemplateBuilder();
  const std::string getName() const;

  class Builder {

  public:
    Builder();
    Builder &name(std::string name);
    Builder &level(CmdLevel level);
    Builder &param(std::string type, const ParamTemplate *checker);
    Builder &trailing(std::string param, const ParamTemplate *checker);
    Builder &command(Command *command);

    const TemplateBuilder *build() const;

  private:
    std::string     _name;
    CmdLevel        _level;
    Command         *_command;
    std::vector<std::pair<std::string, const ParamTemplate *>> _params;
  };

protected:
  TemplateBuilder(
        const std::string &name, CmdLevel level,
        const std::vector<std::pair<std::string, const ParamTemplate *>> &_params,
        Command *command);

private:
    friend class CommandDirector;
    std::string     _name;
    Client          *_sender;
    CmdLevel        _levelNeeded;
    std::vector<std::pair<std::string, const ParamTemplate *>> _params;
    Command *_command;
    bool    check_level(int fd, IRCServer& server)const;
    void fill_param(int fd, std::vector<std::string> &param, IRCServer &server) const;
};
