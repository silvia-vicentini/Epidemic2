void graph() {
    sf::RenderWindow window(sf::VideoMode(800, 600),
                            "Epidemic evolution"); 

    sf::RectangleShape xAxis(
        sf::Vector2f(window.getSize().x,
                     2.f));
    xAxis.setPosition(
        0.f, window.getSize().y /
                 2); 
    xAxis.setFillColor(sf::Color::Black); 

    sf::RectangleShape yAxis(sf::Vector2f(2.f, window.getSize().y));
    yAxis.setPosition(window.getSize().x / 2, 0.f);
    yAxis.setFillColor(sf::Color::Black);

    sf::Font font;
    sf::Text xAxisName("Days", font, 20);
    sf::Text yAxisName("Number of people", font, 20);
    xAxisName.setPosition(window.getSize().x - 100, window.getSize().y - 40);
    yAxisName.setPosition(20, 20);

        sf::Vector2f graphSize(600.f, 400.f);
    sf::Vector2f graphPosition(100.f, 100.f); 
    long int maxXValue = m_T + 1;
    long int maxYValue = N();
    for (float y = 0; y <= maxYValue; y += 20.f) {
      sf::Vertex line[] = {
          sf::Vertex(
              sf::Vector2f(graphPosition.x, graphPosition.y + graphSize.y -
                                                (y / maxYValue) * graphSize.y),
              sf::Color::Black),
          sf::Vertex(sf::Vector2f(graphPosition.x + graphSize.x,
                                  graphPosition.y + graphSize.y -
                                      (y / maxYValue) * graphSize.y),
                     sf::Color::Black)};
      window.draw(line, 2, sf::Lines);
    }
    for (float x = 0; x <= maxXValue; x += 2.f) {
      sf::Vertex line[] = {
          sf::Vertex(
              sf::Vector2f(graphPosition.x + (x / maxXValue) * graphSize.x,
                           graphPosition.y + graphSize.y),
              sf::Color::Black),
          sf::Vertex(
              sf::Vector2f(graphPosition.x + (x / maxXValue) * graphSize.x,
                           graphPosition.y),
              sf::Color::Black)};
      window.draw(line, 2, sf::Lines);
    }

    std::vector<sf::Text> xTicks;
    std::vector<sf::Text> yTicks;
    for (float x = 0.f; x <= maxXValue; x += 2.f) {
      sf::Text tickName;
      tickName.setFont(font);
      tickName.setCharacterSize(14);
      tickName.setFillColor(sf::Color::Black);
      tickName.setString(std::to_string(static_cast<int>(
          x)));
      tickName.setPosition(
          graphPosition.x + (x / maxXValue) * graphSize.x - 10,
          graphPosition.y + graphSize.y + 10);
      xTicks.push_back(tickName); 
    }
    for (float y = 0.f; y <= maxXValue; y += 20.f) {
      sf::Text tickName;
      tickName.setFont(font);
      tickName.setCharacterSize(14);
      tickName.setFillColor(sf::Color::Black);
      tickName.setString(std::to_string(static_cast<int>(
          y)));
      tickName.setPosition(graphPosition.x - 30,
                           graphPosition.y + graphSize.y -
                               (y / maxYValue) * graphSize.y -
                               7); 
      yTicks.push_back(
          tickName);
    }

    sf::VertexArray SusceptibleCurve(sf::LineStrip);  // creo le curve
    sf::VertexArray InfectedCurve(sf::LineStrip);
    sf::VertexArray RecoveryCurve(sf::LineStrip);
    std::vector<Population> population_state_ = evolve();
    for (int i{0}; i <= m_T; ++i) {
      int x = i;
      int yS = population_state_[i].S;
      int yI = population_state_[i].I;
      int yR = population_state_[i].R;
      SusceptibleCurve.append(sf::Vertex(
          sf::Vector2f(
              graphPosition.x + (x / maxXValue) * graphSize.x,
              graphPosition.y + graphSize.y - (yS / maxYValue) * graphSize.y),
          sf::Color::Blue));
      InfectedCurve.append(sf::Vertex(
          sf::Vector2f(
              graphPosition.x + (x / maxXValue) * graphSize.x,
              graphPosition.y + graphSize.y - (yI / maxYValue) * graphSize.y),
          sf::Color::Red));
      RecoveryCurve.append(sf::Vertex(
          sf::Vector2f(
              graphPosition.x + (x / maxXValue) * graphSize.x,
              graphPosition.y + graphSize.y - (yR / maxYValue) * graphSize.y),
          sf::Color::Green));
    }

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
      }
      window.clear(sf::Color::White);

      window.draw(xAxis);
      window.draw(yAxis);
      window.draw(xAxisName);
      window.draw(yAxisName);
      for (auto tick = xTicks.begin(), last = xTick.end(); tick != last; ++tick) {
        window.draw(*tick);
      }
      for (auto tick = yTicks.begin(), last = yTick.end(); tick != last; ++tick) {
        window.draw(*tick);
      }
      window.draw(SusceptibleCurve);
      window.draw(InfectedCurve);
      window.draw(RecoveryCurve);
      window.display();
    }
  };
