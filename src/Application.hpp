#pragma once

#include "Command.hpp"
#include "Network.hpp"
#include "Marker.hpp"

#include <Thor/Graphics/BigSprite.hpp>
#include <Thor/Graphics/BigTexture.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Window.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <map>
#include <memory>
#include <string>

class Application
{
public:
	Application();

	void run();

public:
	static const std::string VERSION;

private:
	void load_resources();
	void load_ui();
	void draw_loading();

	Marker marker_factory(int type, const sf::Color& color, const sf::Vector2f& position);

	void process_event(const sf::Event& event);
	void process_command(Command& command);

	void player_command(Json::Value& data);
	void marker_command(Json::Value& data);

	void update();
	void draw();

	void on_mouse_enter();
	void on_mouse_leave();
	void on_player_rotation();
	void on_map_rotation();

private:
	enum Icons
	{
		ANOMALY         = 22,
		MAG_LIFT        = 23,
		CRASHED_THUMPER = 24,
		CRASHED_LGV     = 25,
		TORNADO         = 29,
		CHOSEN          = 32,
		DROPPOD         = 33,
		REPULSOR        = 37,
		ARES            = 40,
		THUMPER1        = 41,
		THUMPER2        = 42,
		THUMPER3        = 43,
		GENERATOR       = 52,
		INCURSION       = 53,
		ARTILLERY       = 54,
		WARBRINGER      = 56,
		PAYLOAD         = 64,
		TERROMOTO       = 83,
		DEATH_SQUAD     = 84
	};

	sf::RenderWindow m_window;
	sfg::SFGUI m_sfgui;
	Network m_network;

	std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
	std::map<std::string, std::shared_ptr<thor::BigTexture>> m_bigtextures;
	std::map<std::string, sf::IntRect> m_icons;

	std::map<std::string, Marker> m_markers;

	sf::Sprite m_player;
	thor::BigSprite m_map;

	const float m_sidebar_width = 180.f;
	const float m_sidebar_small = 30.f;
	sfg::Desktop m_desktop;
	sfg::Window::Ptr m_ui_window;

	float m_factor;
	float m_zoom;
	bool m_move_view;
	sf::Vector2f m_move_current;
	sf::View m_view;
	sf::View m_ui_view;
	sf::Clock m_timer;
	sf::Time m_frametime;
};
