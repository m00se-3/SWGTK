local module = {
    text = "",
    texture = {}
}

function module.SetText(self, text)
    self.text = text
end

function module.CreateTexture(self)
    self.texture = swgtk.Render:LoadShadedText(self.text, swgtk.Colori.new(0, 0, 0), swgtk.Colori.new(255, 255, 0))
end

return module