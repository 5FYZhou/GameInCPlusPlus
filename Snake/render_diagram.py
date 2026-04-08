from PIL import Image, ImageDraw, ImageFont

WIDTH, HEIGHT = 1600, 1200
BACKGROUND = (30, 30, 30)
BOX_FILL = (50, 50, 50)
BOX_OUTLINE = (200, 200, 200)
TEXT_COLOR = (240, 240, 240)
ARROW_COLOR = (180, 180, 255)

font = ImageFont.load_default()

boxes = {
    'Game': {'pos': (650, 50), 'size': (300, 260), 'lines': [
        'Game',
        '- Snake snake',
        '- Food food',
        '- Scene scene',
        '- Renderer renderer',
        '- Input input',
        '- Direction dir',
        '+ Run()',
        '+ Init()',
        '+ Logic()',
        '+ HandleInput()',
    ]},
    'Renderer': {'pos': (1050, 80), 'size': (280, 220), 'lines': [
        'Renderer',
        '- HANDLE h',
        '+ drawScene()',
        '+ drawScore()',
        '+ drawPause()',
        '+ gotoXY()',
        '+ hideCursor()',
    ]},
    'Input': {'pos': (250, 80), 'size': (260, 140), 'lines': [
        'Input',
        '+ ProcessRunningInput()',
        '+ ProcessPausedInput()',
    ]},
    'SaveSystem': {'pos': (250, 260), 'size': (260, 140), 'lines': [
        'SaveSystem',
        '+ Save()',
        '+ Load()',
        '+ HasSaveData()',
        '+ ClearSaveData()',
    ]},
    'Scene': {'pos': (700, 360), 'size': (280, 140), 'lines': [
        'Scene',
        '- grid',
        '+ Build()',
        '+ GetScene()',
    ]},
    'Snake': {'pos': (450, 520), 'size': (280, 180), 'lines': [
        'Snake',
        '- body',
        '+ Init()',
        '+ CanMove()',
        '+ Move()',
        '+ CheckCollision()',
        '+ GetBody()',
        '+ GetHead()',
    ]},
    'Food': {'pos': (250, 520), 'size': (260, 140), 'lines': [
        'Food',
        '- position',
        '+ Generate()',
        '+ GetPosition()',
        '+ SetPosition()',
    ]},
    'Vector2': {'pos': (950, 560), 'size': (260, 140), 'lines': [
        'Vector2',
        '- x',
        '- y',
        '+ operator+()',
        '+ operator==()',
    ]},
    'GameData': {'pos': (950, 760), 'size': (280, 160), 'lines': [
        'GameData',
        '- mode',
        '- snakeBody',
        '- foodPos',
        '- dir',
        '- score',
    ]},
}

arrows = [
    ('Game', 'Renderer'),
    ('Game', 'Input'),
    ('Game', 'SaveSystem'),
    ('Game', 'Scene'),
    ('Game', 'Snake'),
    ('Game', 'Food'),
    ('Scene', 'Snake'),
    ('Scene', 'Food'),
    ('Scene', 'Vector2'),
    ('Renderer', 'Scene'),
    ('Renderer', 'Vector2'),
    ('Snake', 'Vector2'),
    ('Food', 'Vector2'),
    ('GameData', 'Vector2'),
]

img = Image.new('RGB', (WIDTH, HEIGHT), BACKGROUND)
draw = ImageDraw.Draw(img)

for name, info in boxes.items():
    x, y = info['pos']
    w, h = info['size']
    draw.rectangle([x, y, x + w, y + h], fill=BOX_FILL, outline=BOX_OUTLINE, width=2)
    for idx, line in enumerate(info['lines']):
        draw.text((x + 10, y + 10 + idx * 18), line, font=font, fill=TEXT_COLOR)


def center(name):
    x, y = boxes[name]['pos']
    w, h = boxes[name]['size']
    return (x + w // 2, y + h // 2)


def draw_arrow(a, b):
    ax, ay = center(a)
    bx, by = center(b)
    draw.line([ax, ay, bx, by], fill=ARROW_COLOR, width=2)
    # arrowhead
    dx = bx - ax
    dy = by - ay
    length = (dx**2 + dy**2) ** 0.5
    if length == 0:
        return
    ux, uy = dx / length, dy / length
    left = (bx - ux * 15 - uy * 8, by - uy * 15 + ux * 8)
    right = (bx - ux * 15 + uy * 8, by - uy * 15 - ux * 8)
    draw.polygon([ (bx, by), left, right ], fill=ARROW_COLOR)

for src, dst in arrows:
    draw_arrow(src, dst)

img.save('module_structure.png')
print('Saved module_structure.png')
