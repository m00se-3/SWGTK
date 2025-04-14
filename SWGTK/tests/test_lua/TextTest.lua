local mouse = {}
local background = {}

function OnCreate()
    mouse.texture = swgtk.Render:LoadBlendedWrapText("Hello\nLua!", 0, swgtk.Colori.new(255, 0, 0))
    mouse.angle = 0.0

    swgtk.App:SetFullscreen(true)

    swgtk.Fonts.SetFontStyle(swgtk.Fonts:GetDefaultFont(), swgtk.FontStyle.Italic)

    background = swgtk.Render:LoadShadedWrapText("Lua\nRocks!!", 0, swgtk.Colori.new(0, 0, 0), swgtk.Colori.new(0, 255, 0))

    return true
end

function OnUpdate(dt)
    mouse.pos = swgtk.App:GetMousePos()
    mouse.angle = mouse.angle + (swgtk.math.pi * dt)

    if mouse.angle > swgtk.math.pi2 then
        mouse.angle = mouse.angle - swgtk.math.pi2
    end

    if swgtk.App:IsKeyReleased(swgtk.KeyCode.Space) then
        swgtk.App:SetFullscreen(false)
    end

    local width, height = mouse.texture:GetSize()
    local rect = swgtk.Rectf.new()

    rect.w = width * 2.0
    rect.h = height * 2.0

    rect.x = mouse.pos.x - (rect.w / 2.0)
    rect.y = mouse.pos.y - (rect.h / 2.0)

    swgtk.Render:BufferClear(swgtk.Colorf.new(0.0, 0.0, 0.0))

    swgtk.Render:DrawTexture(background)
    swgtk.Render:DrawTextureRotated(mouse.texture, nil, rect, swgtk.math.radiansToDegrees(mouse.angle))

    return true
end