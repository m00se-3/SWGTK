local mouse = {}
local background = {}

local full = false

function swgtk.OnCreate()
    mouse.texture = swgtk.Render:LoadBlendedWrapText("Hello\nLua!", 0, swgtk.Colori.new(255, 0, 0))
    mouse.angle = 0.0

    swgtk.Fonts.SetFontStyle(swgtk.Fonts:GetDefaultFont(), swgtk.FontStyle.Italic)

    background = swgtk.Render:LoadShadedWrapText("Lua\nRocks!!", 0, swgtk.Colori.new(0, 0, 0), swgtk.Colori.new(0, 255, 0))

    return true
end

function swgtk.OnUpdate(dt)
    mouse.pos = swgtk.App:GetMousePos()
    mouse.angle = mouse.angle + (swgtk.math.pi * dt)

    if mouse.angle > swgtk.math.pi2 then
        mouse.angle = mouse.angle - swgtk.math.pi2
    end

    if swgtk.App:IsKeyReleased(swgtk.KeyCode.Space) then
        full = not full
        swgtk.App:SetFullscreen(full)
    end

    local width, height = mouse.texture:GetSize()
    local rect = swgtk.Rectf.new()

    rect.w = width * 2.0
    rect.h = height * 2.0

    rect.x = mouse.pos.x - (rect.w / 2.0)
    rect.y = mouse.pos.y - (rect.h / 2.0)

    swgtk.Render:BufferClear(swgtk.Colorf.new(0.0, 0.0, 0.0))

    swgtk.Render:DrawTexture(background)
    swgtk.Render:DrawTextureRotated(mouse.texture, nil, rect, swgtk.math.RadiansToDegrees(mouse.angle))

    return true
end