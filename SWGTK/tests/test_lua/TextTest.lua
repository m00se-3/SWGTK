local mouse = {}
local background = {}

function OnCreate()
    mouse.texture = Render:LoadBlendedWrapText("Hello\nLua!", 0, Colori.new(255, 0, 0))
    mouse.angle = 0.0

    Fonts.SetFontStyle(Fonts:GetDefaultFont(), FontStyle.Italic)

    background = Render:LoadShadedWrapText("Lua\nRocks!!", 0, Colori.new(0, 0, 0), Colori.new(0, 255, 0))

    return true
end

function OnUpdate(dt)
    mouse.pos = App:GetMousePos()
    mouse.angle = mouse.angle + (Math.pi * dt)

    if mouse.angle > Math.pi2 then
        mouse.angle = mouse.angle - Math.pi2
    end

    local width, height = mouse.texture:GetSize()
    local rect = Rectf.new()

    rect.w = width * 2.0
    rect.h = height * 2.0

    rect.x = mouse.pos.x - (rect.w / 2.0)
    rect.y = mouse.pos.y - (rect.h / 2.0)

    Render:BufferClear(Colorf.new(0.0, 0.0, 0.0))

    Render:DrawTexture(background)
    Render:DrawTextureRotated(mouse.texture, nil, rect, Math.radiansToDegrees(mouse.angle))

    return true
end