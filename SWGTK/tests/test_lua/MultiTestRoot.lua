local module = require "MultiTestModule"

function swgtk.OnCreate()
    module:SetText("I have used an external module within lua.")
    module:CreateTexture()

    return true
end

function swgtk.OnUpdate(dt)
    swgtk.Render:BufferClear(swgtk.Colorf.new(0.0, 0.0, 0.0))
    swgtk.Render:DrawTexture(module.texture, nil, swgtk.Rectf.new(25, 200, 750, 100))

    return true
end