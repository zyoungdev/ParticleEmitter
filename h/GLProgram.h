/*
  This file is part of Particles.

  fct is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  fct is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with fct.  If not, see <http://www.gnu.org/licenses/>.

  Copyright 2018 Zachary Young
  */

#ifndef __GL_PROGRAM__
#define __GL_PROGRAM__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class GLShader
{
    using String = std::string;

public:
    GLuint type;
    String file_name;
    GLuint compiled;

private:
    std::ifstream file;
    String code;

    GLint success;

public:
    GLShader() {}

    GLShader(const GLuint& t, const String& fn) 
        : type(t), file_name(fn)
    {
        init();
    }

    GLShader(const GLShader& rhs)
    {
        copy(rhs);
    }

    GLShader& operator=(const GLShader& rhs)
    {
        copy(rhs);

        return *this;
    }

    GLShader(GLShader&& o)
        : type{std::move(o.type)},
        file_name{std::move(o.file_name)},
        compiled{std::move(o.compiled)}
    {
        init();
    }

    GLShader& operator=(GLShader&& o)
    {
        type = std::move(o.type);
        file_name = std::move(o.file_name);
        compiled = std::move(o.compiled);
        init();

        return *this;

    }

    ~GLShader()
    {
        glDeleteShader(compiled);
    }

private:
    void openFile()
    {
        file.open(file_name);
    }

    void readFile()
    {
        code.assign(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
    }

    void closeFile()
    {
        file.close();
    }

    void compile()
    {
        const GLchar* shadCode = code.c_str();

        compiled = glCreateShader(type);
        glShaderSource(compiled, 1, &shadCode, NULL);
        glCompileShader(compiled);
    }

    void display_error(const String& err)
    {
        std::cout << err;
    }

    bool compileError()
    {
        glGetShaderiv(compiled, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            String errorStr;
            GLchar errorLog[512];
            glGetShaderInfoLog(compiled, 512, NULL, errorLog);
            errorStr = errorLog;
            display_error("GLShader COMPILE ERROR\n\tFile: " + file_name + "\n\n" + errorStr + "\n");

            return true; 
        }

        return false;
    }

    void shaderCleanup()
    {
        success = 0;
        code.clear();
    }

    void init()
    {
        try
        {
            openFile();
            readFile();
            closeFile();
        }
        catch (std::ifstream::failure e)
        {
            display_error("GLShader FILE ERROR\n\tFile: " + file_name + "\n\tCould not be read.\n\t");
        }

        compile();
        if (compileError())
            std::cout <<  "Could not compile shader";
        
        shaderCleanup();
    }

    void copy(const GLShader& sh)
    {
        type = sh.type;
        file_name = sh.file_name;
        compiled = sh.compiled;
    }
};

class GLProgram
{
    using vec_GLShader = std::vector<GLShader>;
    using String = std::string;

public:
    GLuint prg;

private:
    vec_GLShader compiled_shaders;

    GLint success;
    GLuint shad;

public:
    GLProgram() {}

    GLProgram(const vec_GLShader& sh)
        : compiled_shaders(sh)
    {
        if (init() != 0)
            std::cout << "Could not initialize GLProgram\n";
    }

    GLProgram(const GLProgram& rhs)
    {
        copy(rhs);
    }

    GLProgram& operator=(const GLProgram& rhs)
    {
        copy(rhs);

        return *this;
    }

    GLProgram(GLProgram&& o)
        : prg(std::move(o.prg))
    {}

    GLProgram& operator=(GLProgram&& o)
    {
        prg = std::move(o.prg);

        return *this;
    }

    ~GLProgram() {}

    GLuint program()
    {
        if (is_program())
            return prg;

        std::cout << "ERROR GLProgram.program()\n\tGLProgram has no compiled shader program.\n";

        return 0;
    }

    void program_start()
    {
        if (is_program())
            glUseProgram(prg);
        else
            std::cout << "ERROR GLProgram.program_start()\n\tGLProgram has no compiled shader program.\n";
    }

    void program_stop()
    {
        glUseProgram(0);
    }


private:
    void create_new_program()
    {
        prg = glCreateProgram();
    }

    void attach_shaders()
    {
        for (auto& shader : compiled_shaders)
            glAttachShader(prg, shader.compiled);
    }

    void link()
    {
        glLinkProgram(prg);
    }

    bool is_link_error()
    {
        glGetProgramiv(prg, GL_LINK_STATUS, &success);

        if (!success)
        {
            String errorStr;
            GLchar errorLog[512];
            glGetProgramInfoLog(prg, 512, NULL, errorLog);
            errorStr = errorLog;
            display_error("GLProgram LINK ERROR\n\tProgram could not be linked.\n\n" + errorStr + "\n");

            return true;
        }

        return false;
    }

    void is_link_success()
    {
        std::cout << "Created GLProgram with\n\t";

        for (const auto& shader : compiled_shaders)
            std::cout << shader.file_name << " ";

        std::cout << "\n\n";
    }

    void cleanup_linking()
    {
        for (auto& shader : compiled_shaders)
            glDetachShader(prg, shader.compiled);

        compiled_shaders = {};
        success = 0;
    }

    void display_error(const String& err)
    {
        std::cout << err;
    }

    bool is_program()
    {
        return glIsProgram(prg);
    }

    void copy(const GLProgram& pr)
    {
        prg = pr.prg;
    }

    int init()
    {
        create_new_program();

        attach_shaders();
        link();

        if (is_link_error())
            return 1;

        is_link_success();
        cleanup_linking();

        return 0;
    }
};

#endif
