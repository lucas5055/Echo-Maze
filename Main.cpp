#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <string>

int main()
{

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Echo Maze");
    window.setFramerateLimit(60);

    float energy_size_x = 150.f;

    // forms

    sf::RectangleShape rectangle;
    sf::CircleShape circle;
    circle.setPointCount(100);

    // Text
    sf::Font font("Minecraft.ttf");
    sf::Text text(font, "Energy");
    // int file_count = 1;

    // Sound
    sf::SoundBuffer pulse_buffer("Sounds/Pulse/Pulse_Slowed(1).mp3");
    sf::Sound pulse_sound(pulse_buffer);
    
    sf::SoundBuffer backgroung_buffer("Sounds/Ambient/Ambient_(2).mp3"); //9, 5, 4 (3,2 se editar),
    sf::Sound background_music_sound(backgroung_buffer);
    
    //sf::SoundBuffer pulse_recharge_buffer ("Sounds/Recharge/Recharging_(1).mp3");
    //sf::Sound pulse_recharging_sound (pulse_recharge_buffer);
    
    sf::SoundBuffer pulse_recharged_buffer ("Sounds/Recharge/Recharge_done.mp3");
    sf::Sound pulse_recharged_sound (pulse_recharged_buffer);
    
    // Itens
    sf::Clock clock;
    sf::Clock music_clock;
    int pulse_count = 0;

    /*sf::Texture texture (filename);
    sf::Sprite sprite (texture);
    sf::FloatRect bounds = sprite.getGlobalBounds();*/

    /*float pos_y;
    float pos_x;*/

    // Pulse
    bool pulse = false;
    bool b_music = false;
    bool pulse_recharge = false;
    bool pulse_recharged = false;

    // Circle "brightness"
    const float INSIDE_START = 40.f;
    const float OUTLINE_START = 80.f;

    // 1º Circle
    float radius = 0.f;
    float radius_speed = 100.f;
    float inside_fade = INSIDE_START;
    float outline_fade = OUTLINE_START;
    float inside_fade_speed = 25.f;
    float outline_fade_speed = 25.f;

    // 2º Circle
    float radius_2 = 0.f;
    float radius_speed_2 = 100.f;
    float inside_fade_2 = INSIDE_START;
    float outline_fade_2 = OUTLINE_START;
    float inside_fade_speed_2 = 45.f;
    float outline_fade_speed_2 = 30.f;

    // 3º Circle
    float radius_3 = 0.f;
    float radius_speed_3 = 100.f;
    float inside_fade_3 = INSIDE_START;
    float outline_fade_3 = OUTLINE_START;
    float inside_fade_speed_3 = 55.f;
    float outline_fade_speed_3 = 40.f;

    // Lambda
    auto DrawCircle = [&](float position_x, float position_y, float radius, float out_fade, float in_fade)
    {
        circle.setRadius(radius);
        circle.setPosition({position_x, position_y});
        circle.setOrigin(circle.getGeometricCenter());

        std::uint8_t in_f = static_cast<std::uint8_t>(std::clamp(in_fade, 0.f, 255.f));
        std::uint8_t out_f = static_cast<std::uint8_t>(std::clamp(out_fade, 0.f, 255.f));

        circle.setFillColor(sf::Color(255, 255, 255, in_f));
        circle.setOutlineThickness(5.f);
        circle.setOutlineColor(sf::Color(255, 255, 255, out_f));

        window.draw(circle);
    };

    auto DrawCircle_2 = [&](float position_x, float position_y, float radius, float out_fade, float in_fade)
    {
        circle.setRadius(radius);
        circle.setPosition({position_x, position_y});
        circle.setOrigin(circle.getGeometricCenter());

        std::uint8_t in_f = static_cast<std::uint8_t>(std::clamp(in_fade, 0.f, 255.f));
        std::uint8_t out_f = static_cast<std::uint8_t>(std::clamp(out_fade, 0.f, 255.f));

        circle.setFillColor(sf::Color(255, 255, 255, in_f));
        circle.setOutlineThickness(5.f);
        circle.setOutlineColor(sf::Color(255, 255, 255, out_f));

        window.draw(circle);
    };

    auto DrawCircle_3 = [&](float position_x, float position_y, float radius, float out_fade, float in_fade)
    {
        circle.setRadius(radius);
        circle.setPosition({position_x, position_y});
        circle.setOrigin(circle.getGeometricCenter());

        std::uint8_t in_f = static_cast<std::uint8_t>(std::clamp(in_fade, 0.f, 255.f));
        std::uint8_t out_f = static_cast<std::uint8_t>(std::clamp(out_fade, 0.f, 255.f));

        circle.setFillColor(sf::Color(255, 255, 255, in_f));
        circle.setOutlineThickness(5.f);
        circle.setOutlineColor(sf::Color(255, 255, 255, out_f));

        window.draw(circle);
    };

    auto DrawEnergy = [&](float position_x, float position_y, float size_x)
    {
        rectangle.setPosition({position_x, position_y});
        rectangle.setFillColor(sf::Color(0, 154, 31));
        rectangle.setSize({size_x, 20.f});
        rectangle.setOutlineColor(sf::Color(0, 154, 31));
        rectangle.setOutlineThickness(2.f);
        rectangle.setOrigin(rectangle.getGeometricCenter());
        window.draw(rectangle);
    };

    auto DrawText = [&](float position_x, float position_y)
    {
        text.setScale({0.7f, 0.7f});
        text.setPosition({position_x, position_y});
        text.setOrigin({0, 0});
        text.setCharacterSize(30);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::White);
        window.draw(text);
    };

    /*auto DrawCharcter = [&] (float position_x, float position_y, float move_x, float move_y)
    {
        sprite.setScale({0.1f, 0.1f});
        sprite.setPosition({position_x, position_y});
       auto pos= sprite.getPosition();
        sprite.setOrigin(bounds.size.componentWiseMul({0.5f,0.5f}));
        window.draw(sprite);
    };*/

    while (window.isOpen())
    {
        float window_w = window.getView().getSize().x;
        float window_h = window.getView().getSize().y;

        float dt = clock.restart().asSeconds();
        
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Enter and energy_size_x > 50.f)
                {
                    pulse = true;

                    radius = radius_2 = radius_3 = 0.f;

                    inside_fade = inside_fade_2 = inside_fade_3 = INSIDE_START;
                    outline_fade = outline_fade_2 = outline_fade_3 = OUTLINE_START;

                    if (energy_size_x > 50)
                    {
                        energy_size_x = energy_size_x - 50.f;
                    }
                    pulse_sound.play();
                }

                /*if (key->scancode == sf::Keyboard::Scancode::Space)
                {
                    if (dt >= 0.015000f)
                    {
                        pos_x = pos_x - 100.f;
                        pos_y = pos_y - 100.f;

                        DrawCharcter(window_w / 2,window_h/2, pos_x, pos_y);
                    }

                }*/
            }
        }

        window.clear(sf::Color::Black);

        if (pulse)
        {
            // 1º Circle
            if (radius <= 500.f)
            {
                radius += radius_speed * dt;
                inside_fade -= inside_fade_speed * dt;
                outline_fade -= outline_fade_speed * dt;

                DrawCircle(window_w / 2, window_h / 2, radius, outline_fade, inside_fade);
            }

            // 2º Circle — starts at 100
            if (radius >= 100.f and radius_2 <= 400.f)
            {
                radius_2 += radius_speed_2 * dt;
                inside_fade_2 -= inside_fade_speed_2 * dt;
                outline_fade_2 -= outline_fade_speed_2 * dt;

                DrawCircle_2(window_w / 2, window_h / 2, radius_2, outline_fade_2, inside_fade_2);
            }

            // 3º Circle — starts at após 200
            if (radius >= 200.f and radius_3 <= 300.f)
            {
                radius_3 += radius_speed_3 * dt;
                inside_fade_3 -= inside_fade_speed_3 * dt;
                outline_fade_3 -= outline_fade_speed_3 * dt;

                DrawCircle_3(window_w / 2, window_h / 2, radius_3, outline_fade_3, inside_fade_3);
            }

            // reset
            if (radius >= 500.f and radius_2 >= 400.f and radius_3 >= 300.f)
            {
                pulse = false;

                radius = radius_2 = radius_3 = 0.f;
                inside_fade = inside_fade_2 = inside_fade_3 = INSIDE_START;
                outline_fade = outline_fade_2 = outline_fade_3 = OUTLINE_START;
            }
        }
        if (energy_size_x < 150.f)
        {
            pulse_recharge = true;
            pulse_recharged = false;
            energy_size_x += energy_size_x * (dt / 15);
            /*if(pulse_recharge == true)
            {
                pulse_recharging_sound.setLooping(pulse_recharge);
                pulse_recharging_sound.play();
                music_clock.restart();  //Acho que n precisa disso
            }*/
        }
        if (energy_size_x <= 50.f)
        {
            energy_size_x += energy_size_x  * (dt / 5);
        }
        std::cout <<energy_size_x<< std::endl;
        if (energy_size_x >= 149.500f and energy_size_x <= 149.999f)
        {
            pulse_recharged_sound.play();
            pulse_count = pulse_count + 1;
            /*if (pulse_count == 1)
            {
                pulse_recharged_sound.stop();
                pulse_count = 0;
            }*/
        }
        if (b_music == false)
        {
            b_music = true;
            background_music_sound.setLooping(b_music);
            background_music_sound.play();
            music_clock.restart(); //Acho que n precisa disso
        }
        /*Penso em colocar um efeito de pulse carregado, como o de impulso carregado de doom e pelo jeito que e codado hoje eu posso usar as "paradas" que o carregamento da (por conta das condicionais e dt) assim aplicando 3 pulsos
        e talvez mudar a mecanica do pulse para apenas um "flash" assim como em funtime foxy and fnaf 5
        (isso tbm abre posibilidade para um longo som de carregamento de "flash" assim como também em funtime foxy mais facil do q por um texto pra mostar a energia e asim eu posso fazer um carregamento flente de energia)*/
        DrawEnergy(window_w / 1.15, window_h / 9.5, energy_size_x);

        DrawText(window_w / 1.5, window_h / 12.5);

        // DrawCharcter(window_w / 2,window_h/2, pos_x, pos_y );

        window.display();
    }

    return 0;
}
