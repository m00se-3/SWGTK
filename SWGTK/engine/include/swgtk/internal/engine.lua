-- We are running the main loop from within Lua itself.
---@diagnostic disable-next-line: lowercase-global
function main()
    if swgtk.OnCreate() then
        while swgtk.App:IsAppRunning() do
            swgtk.App:EventsAndTimeStep()
            if not swgtk.OnUpdate(swgtk.App.DeltaTime:GetSeconds()) then swgtk.App:CloseApp() end
            swgtk.Render:BufferPresent()
        end
    end
end