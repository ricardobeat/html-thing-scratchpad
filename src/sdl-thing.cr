require "sdl"

SDL.init(SDL::Init::VIDEO)
at_exit { SDL.quit }

window = SDL::Window.new("SDL tutorial", 640, 480,
  SDL::Window::Position::UNDEFINED,
  SDL::Window::Position::UNDEFINED,
  SDL::Window::Flags::RESIZABLE)
renderer = SDL::Renderer.new(window)

window.bordered = 0

width, height = window.size

loop do
  window.swap_buffers
  case event = SDL::Event.wait
  when SDL::Event::Quit
    break
  when SDL::Event::Window
    case event.event
    when LibSDL::WindowEventID::SIZE_CHANGED.value
      puts "resized"
    end
  end

  # clear sreen in white
  renderer.draw_color = {255, 255, 255, 255}
  renderer.clear

  # centered red rectangle
  renderer.draw_color = {255, 0, 0, 255}
  renderer.fill_rect(width / 4, height / 4, width / 2, height / 2)

  # outlined green rectangle
  renderer.draw_color = {0, 255, 0, 255}
  renderer.draw_rect(width / 6, height / 6, width * 2 / 3, height * 2 / 3)

  # blue horizontal line
  renderer.draw_color = {0, 0, 255, 255}
  renderer.draw_line(0, height / 2, width, height / 2)

  # vertical line of yellow dots
  renderer.draw_color = {255, 255, 0, 255}
  0.step(height, 4) do |i|
    renderer.draw_point(width / 2, i)
  end

  renderer.present
end
