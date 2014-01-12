/*
 * myapp.cpp
 * 
 * Copyright 2014 Petri J. Vaisanen <petri.j.vaisanen@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
#include <iostream>

#include "myapp.h"

MyApp::MyApp(): _helpRequested(false)
{
}

MyApp::~MyApp()
{
    
}

/*
 * Initializes the application and all registered subsystems. 
 * Subsystems are always initialized in the exact same order in which 
 * they have been registered. Overriding implementations must call the 
 * base class implementation.
 */
void MyApp::initialize(Poco::Util::Application& self)
{
    loadConfiguration(); // load default configuration files, if present
    Poco::Util::Application::initialize(self);
    // add your own initialization code here
}

void MyApp::reinitialize(Poco::Util::Application& self)
{
    Poco::Util::Application::reinitialize(self);
    // add your own reinitialization code here
}

/*
 * Uninitializes the application and all registered subsystems. 
 * Subsystems are always uninitialized in reverse order in which they 
 * have been initialized. Overriding implementations must call the base 
 * class implementation.
 */
void MyApp::uninitialize()
{
    // add your own uninitialization code here
    Poco::Util::Application::uninitialize();
}

/* Called before command line processing begins. If a subclass wants 
 * to support command line arguments, it must override this method. 
 * The default implementation does not define any options itself, but 
 * calls defineOptions() on all registered subsystems. Overriding 
 * implementations should call the base class implementation.
 */
void MyApp::defineOptions(Poco::Util::OptionSet& options)
{
    Poco::Util::Application::defineOptions(options);

    options.addOption(
        Poco::Util::Option("help", "h", "display help information on command line arguments")
            .required(false)
            .repeatable(false)
            .callback(Poco::Util::OptionCallback<MyApp>(this, &MyApp::handleHelp)));

    options.addOption(
        Poco::Util::Option("define", "D", "define a configuration property")
            .required(false)
            .repeatable(true)
            .argument("name=value")
            .callback(Poco::Util::OptionCallback<MyApp>(this, &MyApp::handleDefine)));
            
    options.addOption(
        Poco::Util::Option("config-file", "f", "load configuration data from a file")
            .required(false)
            .repeatable(true)
            .argument("file")
            .callback(Poco::Util::OptionCallback<MyApp>(this, &MyApp::handleConfig)));

    options.addOption(
        Poco::Util::Option("bind", "b", "bind option value to test.property")
            .required(false)
            .repeatable(false)
            .argument("value")
            .binding("test.property"));
}

/* Called when the option with the given name is encountered during 
 * command line arguments processing. The default implementation does 
 * option validation, bindings and callback handling. Overriding 
 * implementations must call the base class implementation.
 * Template:
 * void MyApp::handleOption(const std::string & name, const std::string & value)
 * {
 *     Poco::Util::Application::handleOption(name , value);
 * }
 */
void MyApp::handleHelp(const std::string& name, const std::string& value)
{
    _helpRequested = true;
    displayHelp();
    stopOptionsProcessing();
}

void MyApp::handleDefine(const std::string& name, const std::string& value)
{
    defineProperty(value);
}

void MyApp::handleConfig(const std::string& name, const std::string& value)
{
    loadConfiguration(value);
}

void MyApp::displayHelp()
{
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("A sample application that demonstrates some of the features of the Poco::Util::Application class.");
    helpFormatter.format(std::cout);
}

void MyApp::defineProperty(const std::string& def)
{
    std::string name;
    std::string value;
    std::string::size_type pos = def.find('=');
    if (pos != std::string::npos)
    {
        name.assign(def, 0, pos);
        value.assign(def, pos + 1, def.length() - pos);
    }
    else name = def;
    config().setString(name, value);
}

/* Initializes the application and all registered subsystems. 
 * Subsystems are always initialized in the exact same order in which 
 * they have been registered. Overriding implementations must call the 
 * base class implementation.
 */
int MyApp::main(const std::vector<std::string>& args)
{
    if (!_helpRequested)
    {
        logger().information("Arguments to main():");
        for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
        {
            logger().information(*it);
        }
        logger().information("Application properties:");
        printProperties("");
    }
    return Poco::Util::Application::EXIT_OK;
}

void MyApp::printProperties(const std::string& base)
{
    Poco::Util::AbstractConfiguration::Keys keys;
    config().keys(base, keys);
    if (keys.empty())
    {
        if (config().hasProperty(base))
        {
            std::string msg;
            msg.append(base);
            msg.append(" = ");
            msg.append(config().getString(base));
            logger().information(msg);
        }
    }
    else
    {
        for (Poco::Util::AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it)
        {
            std::string fullKey = base;
            if (!fullKey.empty()) fullKey += '.';
            fullKey.append(*it);
            printProperties(fullKey);
        }
    }
}
