require "./html-thing/*"
require "../crsfml/src/crsfml.cr"

module Html::Thing
  # TODO Put your code here
end

mode = SF::VideoMode.new(800, 600)
window = SF::RenderWindow.new(mode, "Is this HTML?")
window.vertical_sync_enabled = true

class Rect < SF::RectangleShape
end

boxes = [] of Rect

connection = Rect.new({window.size[0] / 2, window.size[1] / 2})
connection.origin = {-0.05, -0.05}
connection.position = {10, 10}
connection.fill_color = SF.color(255, 128, 0)

boxes << connection

events = Channel(SF::Event).new
drawing = Channel(Int32).new

def draw_window(window, boxes)
  window.clear SF::Color.new(255, 0, 0)

  boxes.each do |box|
    window.draw box
  end

  window.display
end

while window.open?
  while event = window.poll_event
    case event
    when SF::Event::Closed
      window.close
    when SF::Event::KeyPressed
      if event.code.escape?
        window.close
      end
    when SF::Event::Resized
      window.size = {event.width, event.height}
      # window.size = SF::Vector2.new(event.width.to_i32, event.height.to_i32)
      # rect = SF::FloatRect(0, 0, event.size.width, event.size.height)SF::FloatRect(0, 0, event.size.width, event.size.height)
      # window.setView(SF::View(r))
    end
  end
  spawn draw_window(window, boxes)
end
