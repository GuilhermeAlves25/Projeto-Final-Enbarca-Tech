# Projeto: Controle de Display OLED SSD1306 com Raspberry Pi Pico

## Descrição
Este projeto demonstra como utilizar a Raspberry Pi Pico para controlar um display OLED SSD1306 via protocolo I2C. Além disso, o código inclui interação com botões e LEDs, permitindo ao usuário modificar a exibição na tela com base nas entradas dos botões.

## Componentes Utilizados
- Raspberry Pi Pico
- Display OLED SSD1306 (I2C)
- LEDs
- Botões

## Bibliotecas Necessárias
Para compilar e executar o projeto corretamente, será necessário instalar as seguintes bibliotecas:
- `pico/stdlib.h`
- `hardware/i2c.h`
- `ssd1306.h` (se aplicável, driver para o display OLED)

## Conexões
As conexões entre a Raspberry Pi Pico e o display SSD1306 devem ser feitas da seguinte forma:

| Raspberry Pi Pico | Display OLED SSD1306 |
|------------------|---------------------|
| VCC             | 3.3V                |
| GND             | GND                 |
| GP4 (I2C0 SDA)  | SDA                 |
| GP5 (I2C0 SCL)  | SCL                 |

As conexões dos botões e LEDs devem seguir a lógica do código, ligando os pinos correspondentes.

## Como Usar
1. Clone este repositório:
   ```bash
   git clone https://github.com/seu-usuario/seu-repositorio.git
   ```
2. Compile e grave o código na Raspberry Pi Pico usando um ambiente compatível (VS Code, Thonny, etc.).
3. Conecte os componentes corretamente.
4. Execute o código e observe a interação do display com os botões e LEDs.

## Possíveis Melhorias
- Adicionar mais elementos gráficos ao display.
- Implementar anímações.
- Melhorar a interação com os botões.

## Autor
Guilherme Alves Da Silva

##link para o video demonstrativo:
(https://youtu.be/8558GKE03Sk?si=qTQylBmyYGe8X5Qv)


