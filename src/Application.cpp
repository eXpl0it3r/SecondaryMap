#include "Application.hpp"

#include "utility.hpp"

#include <json/json.h>

#include <SFML/System/Err.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <deque>
#include <iostream>
#include <sstream>

#include <cmath>

const std::string Application::VERSION = "0.1";

Application::Application() :
	m_window{sf::VideoMode{1024, 768}, "Secondary Map"},
	m_factor{2.f},
	m_zoom{0.f},
	m_move_view{false}
{
	m_window.setFramerateLimit(30);
	m_view = m_window.getDefaultView();
	m_ui_view = m_window.getDefaultView();

	draw_loading();
	load_resources();
	load_ui();

	m_player.setTexture(*m_textures["markers"]);
	m_player.setTextureRect(m_icons["player"]);
	m_player.setOrigin({m_player.getLocalBounds().width/2, m_player.getLocalBounds().height/2});
	m_player.setColor({0xFF, 0xC0, 0x40});

	m_map.setTexture(*m_bigtextures["map"]);
	m_map.setOrigin({4610.f, 4610.f});
}

void Application::load_resources()
{
	// Load Resources
	m_textures.emplace("markers", std::shared_ptr<sf::Texture>(new sf::Texture));
	if(!m_textures["markers"]->loadFromFile("assets/img/map_markers.png"))
		sf::err() << "Couldn't load assets/img/map_markers.png" << std::endl;

	m_textures.emplace("icons", std::shared_ptr<sf::Texture>(new sf::Texture));
	if(!m_textures["icons"]->loadFromFile("assets/img/map_icons.png"))
		sf::err() << "Couldn't load assets/img/map_icons.png" << std::endl;

	m_textures.emplace("battleframe", std::shared_ptr<sf::Texture>(new sf::Texture));
	if(!m_textures["battleframe"]->loadFromFile("assets/img/icons_battleframes.png"))
		sf::err() << "Couldn't load assets/img/icons_battleframes.png" << std::endl;

	m_textures.emplace("deployables", std::shared_ptr<sf::Texture>(new sf::Texture));
	if(!m_textures["deployables"]->loadFromFile("assets/img/icons_deployables.png"))
		sf::err() << "Couldn't load assets/img/icons_deployables.png" << std::endl;

	m_textures.emplace("vehicles", std::shared_ptr<sf::Texture>(new sf::Texture));
	if(!m_textures["vehicles"]->loadFromFile("assets/img/icons_vehicles.png"))
		sf::err() << "Couldn't load assets/img/icons_vehicles.png" << std::endl;

	m_textures.emplace("outpost", std::shared_ptr<sf::Texture>(new sf::Texture));
	if(!m_textures["outpost"]->loadFromFile("assets/img/outpost.png"))
		sf::err() << "Couldn't load assets/img/outpost.png" << std::endl;

	m_bigtextures.emplace("map", std::shared_ptr<thor::BigTexture>(new thor::BigTexture));
	if(!m_bigtextures["map"]->loadFromFile("assets/img/dummy.png")) //0.7.1710-New-Eden-Minimap.png"))
		sf::err() << "Couldn't load assets/img/0.7.1710-New-Eden-Minimap.png" << std::endl;

	// Markers
	m_icons.emplace("player", sf::IntRect{32, 32, 32, 32});
	m_icons.emplace("dead", sf::IntRect{0, 0, 32, 32});

	// Battleframe
	m_icons.emplace("medic", sf::IntRect{0, 0, 63, 63});
	m_icons.emplace("recon", sf::IntRect{63, 0, 64, 63});
	m_icons.emplace("unkown", sf::IntRect{12, 70, 38, 48});
	m_icons.emplace("engineer", sf::IntRect{68, 63, 54, 64});
	m_icons.emplace("assault", sf::IntRect{0, 127, 63, 62});
	m_icons.emplace("dreadnaught", sf::IntRect{68, 127, 54, 64});

	// Depolyables
	m_icons.emplace("turret", sf::IntRect{133, 11, 54, 43});
	m_icons.emplace("sinbeacon", sf::IntRect{5, 70, 53, 52});
	m_icons.emplace("aaturret", sf::IntRect{67, 71, 57, 48});
	m_icons.emplace("battleframe", sf::IntRect{129, 71, 64, 48});
	m_icons.emplace("gilder", sf::IntRect{195, 67, 56, 58});
	m_icons.emplace("garage", sf::IntRect{1, 193, 60, 60});

	// Vehicles
	m_icons.emplace("dropship", sf::IntRect{127, 0, 64, 64});
	m_icons.emplace("lgv", sf::IntRect{191, 0, 64, 64});

	// Icons
	m_icons.emplace("crashedlgv", sf::IntRect{0, 0, 64, 64});
	m_icons.emplace("crashedthumper", sf::IntRect{64, 0, 64, 64});
	m_icons.emplace("race", sf::IntRect{128, 0, 64, 64});
	m_icons.emplace("lgv2", sf::IntRect{64, 0, 64, 64});
	m_icons.emplace("radio", sf::IntRect{64, 64, 64, 64});
	m_icons.emplace("sintoweractive", sf::IntRect{128, 64, 64, 64});
	m_icons.emplace("sintowertaken", sf::IntRect{0, 128, 64, 64});
	m_icons.emplace("sintower", sf::IntRect{64, 128, 64, 64});
	m_icons.emplace("tornado", sf::IntRect{128, 128, 64, 64});
	m_icons.emplace("watchtoweractive", sf::IntRect{64, 192, 64, 64});
	m_icons.emplace("watchtowertaken", sf::IntRect{128, 192, 64, 64});
	m_icons.emplace("watchtower", sf::IntRect{0, 256, 64, 64});
	m_icons.emplace("chosen", sf::IntRect{128, 256, 64, 64});
	m_icons.emplace("anomaly", sf::IntRect{0, 320, 64, 64});
	m_icons.emplace("droppod", sf::IntRect{64, 320, 64, 64});
	m_icons.emplace("baneclaw", sf::IntRect{128, 320, 64, 64});
	m_icons.emplace("ares", sf::IntRect{0, 384, 64, 64});
	m_icons.emplace("thumper5", sf::IntRect{128, 384, 64, 64});
	m_icons.emplace("thumper4", sf::IntRect{0, 448, 64, 64});
	m_icons.emplace("thumper3", sf::IntRect{64, 448, 64, 64});
	m_icons.emplace("thumper2", sf::IntRect{128, 448, 64, 64});
	m_icons.emplace("thumper1", sf::IntRect{0, 512, 64, 64});
	m_icons.emplace("ares5", sf::IntRect{64, 512, 64, 64});
	m_icons.emplace("ares4", sf::IntRect{128, 512, 64, 64});
	m_icons.emplace("ares3", sf::IntRect{0, 576, 64, 64});
	m_icons.emplace("ares2", sf::IntRect{64, 576, 64, 64});
	m_icons.emplace("ares1", sf::IntRect{128, 576, 64, 64});
	m_icons.emplace("generator", sf::IntRect{0, 640, 64, 64});
	m_icons.emplace("incursion", sf::IntRect{64, 640, 64, 64});
}

void Application::load_ui()
{
	m_ui_window = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_ui_window->SetAllocation(sf::FloatRect(m_window.getSize().x-m_sidebar_small, 0.f, m_sidebar_width, m_window.getSize().y));
	m_ui_window->GetSignal(sfg::Button::OnMouseEnter).Connect(&Application::on_mouse_enter, this);
	m_ui_window->GetSignal(sfg::Button::OnMouseLeave).Connect(&Application::on_mouse_leave, this);

	m_desktop.Add(m_ui_window);
}

void Application::draw_loading()
{
	sf::Texture loading;
	loading.loadFromFile("assets/img/loading.png");
	sf::Sprite sprite(loading);
	sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
	sprite.setPosition(m_window.getSize().x/2.f, m_window.getSize().y/2.f);

	m_window.clear();
	m_window.draw(sprite);
	m_window.display();
}

Marker Application::marker_factory(int type, const sf::Color& color, const sf::Vector2f& position)
{
	switch(type)
	{
		case ANOMALY:
			return Marker{m_textures["icons"], m_icons["anomaly"], color, position};
			break;
		case CRASHED_LGV:
			return Marker{m_textures["icons"], m_icons["crashedlgv"], color, position};
			break;
		case CRASHED_THUMPER:
			return Marker{m_textures["icons"], m_icons["crashedthumper"], color, position};
			break;
		case TORNADO:
			return Marker{m_textures["icons"], m_icons["tornado"], color, position};
			break;
		case CHOSEN:
		case DEATH_SQUAD:
			return Marker{m_textures["icons"], m_icons["chosen"], color, position};
			break;
		case DROPPOD:
			return Marker{m_textures["icons"], m_icons["droppod"], color, position};
			break;
		case THUMPER1:
		case THUMPER2:
		case THUMPER3:
			return Marker{m_textures["icons"], m_icons["thumper1"], color, position};
			break;
		case GENERATOR:
			return Marker{m_textures["icons"], m_icons["generator"], color, position};
			break;
		case INCURSION:
		case WARBRINGER:
			return Marker{m_textures["icons"], m_icons["incursion"], color, position};
			break;
		case TERROMOTO:
			return Marker{m_textures["icons"], m_icons["race"], color, position};
			break;
		case ARES:
			return Marker{m_textures["icons"], m_icons["ares"], color, position};
			break;
		default:
			break;
	}

	return Marker{m_textures["battleframe"], m_icons["unkown"], color, position};
}

void Application::run()
{
	while(m_window.isOpen())
	{
		update();
		draw();
	}
}

void Application::update()
{
	m_frametime = m_timer.restart();

	sf::Event event;

	while(m_window.pollEvent(event))
	{
		m_desktop.HandleEvent(event);
		process_event(event);
	}

	m_desktop.Update(m_frametime.asSeconds());

	std::deque<Command> commands = m_network.update();

	while(!commands.empty())
	{
		process_command(commands.front());
		commands.pop_front();
	}

	m_view.setCenter(m_player.getPosition());
}

void Application::process_event(const sf::Event& event)
{
	if(event.type == sf::Event::Closed)
		m_window.close();
	else if(event.type == sf::Event::Resized)
	{
		sf::Vector2f size = static_cast<sf::Vector2f>(m_window.getSize());

		// Minimum size
		if(size.x < 800)
			size.x = 800;
		if(size.y < 600)
			size.y = 600;

		// Apply possible size changes
		m_window.setSize(static_cast<sf::Vector2u>(size));

		// Update UI view
		m_ui_view = sf::View(sf::FloatRect(0.f, 0.f, size.x, size.y));

		// Reset grid view
		m_view.setCenter(size/2.f);
		m_view.setSize(size);

		m_ui_window->SetAllocation(sf::FloatRect(size.x-m_sidebar_small, 0.f, m_sidebar_width, size.y));
	}
	else if(event.type == sf::Event::MouseWheelMoved)
		m_view.zoom(1.f+(-0.05f*event.mouseWheel.delta));
	else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Add)
	{
		m_factor += 0.01f;
		std::cout << m_factor << std::endl;
	}
	else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Subtract)
	{
		m_factor -= 0.01f;
		std::cout << m_factor << std::endl;
	}
}

void Application::process_command(Command& command)
{
	if(!command.failed())
	{
		Json::Value data = command.data();

		if(command.tokens().front() == "player")
		{
			player_command(data);
		}
		else if(command.tokens().front() == "marker")
		{
			marker_command(data);
		}
	}
}

void Application::player_command(Json::Value& data)
{
	//std::cout << "x: " << data["x"].asFloat() << " / y: " << data["y"].asFloat() << std::endl;
	m_player.setPosition({data["x"].asFloat()*m_factor, -data["y"].asFloat()*m_factor});
	m_player.setRotation(data["rotation"].asFloat()*180/utility::PI);
}

void Application::marker_command(Json::Value& data)
{
	std::cout << "Marker " << data["id"].asInt() << ": " << data["state"].asString() << " / " << data["type"].asInt() << " / (" << data["x"].asFloat() << ", " << data["y"].asFloat() << ")" << std::endl;

	if(data["state"].asString() == "clear")
	{
		std::cout << "Markers: Clear" << std::endl;
		m_markers.clear();
	}
	else if(data["state"].asString() == "sync")
	{
		for(auto& marker : data["markers"])
		{
			std::cout << "MarkerId: " << marker["id"].asInt() << std::endl;
			m_markers.emplace(marker["id"].asInt(), marker_factory(marker["type"].asInt(), sf::Color(marker["r"].asInt(), marker["g"].asInt(), marker["b"].asInt(), marker["a"].asInt()), sf::Vector2f{marker["x"].asFloat()*m_factor, -marker["y"].asFloat()*m_factor}));
		}
	}
	else if(data["state"].asString() == "add")
	{
		m_markers.emplace(data["id"].asInt(), marker_factory(data["type"].asInt(), sf::Color(data["r"].asInt(), data["g"].asInt(), data["b"].asInt(), data["a"].asInt()), sf::Vector2f{data["x"].asFloat()*m_factor, -data["y"].asFloat()*m_factor}));
	}
	else if(data["state"].asString() == "update")
	{
		auto it = m_markers.find(data["id"].asInt());

		if(it != m_markers.end()) // Not found
			it->second.position({data["x"].asFloat()*m_factor, -data["y"].asFloat()*m_factor});
		else // Create it
			m_markers.emplace(data["id"].asInt(), marker_factory(data["type"].asInt(), sf::Color(data["r"].asInt(), data["g"].asInt(), data["b"].asInt(), data["a"].asInt()), sf::Vector2f{data["x"].asFloat()*m_factor, -data["y"].asFloat()*m_factor}));
	}
	else if(data["state"].asString() == "remove")
	{
		auto it = m_markers.find(data["id"].asInt());

		if(it != m_markers.end())
			m_markers.erase(it);
	}
}

void Application::draw()
{
	m_window.clear();
	m_window.setView(m_view);
	m_window.draw(m_map);

	for(auto& marker : m_markers)
		m_window.draw(marker.second);

	m_window.draw(m_player);

	// Draw GUI
	m_window.setView(m_ui_view);
	m_sfgui.Display(m_window);
	m_window.display();
}

void Application::on_mouse_enter()
{
	m_ui_window->SetAllocation(sf::FloatRect(m_window.getSize().x-m_sidebar_width, 0.f, m_sidebar_width, m_window.getSize().y));
}

void Application::on_mouse_leave()
{
	m_ui_window->SetAllocation(sf::FloatRect(m_window.getSize().x-m_sidebar_small, 0.f, m_sidebar_width, m_window.getSize().y));
}
