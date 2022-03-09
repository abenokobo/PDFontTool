import "CoreLibs/object"
import "CoreLibs/graphics"

local pd = playdate
local gfx = pd.graphics


--
local TEXT_TITLE = "- PDFontTool TEST -"
local TEXT_TEMPLATE =
{
    "",
    "",
    "天の原ふりさけみれば春日なる",
    "三笠の山にいでし月かも 安倍仲麿"
}
local FONT_PATH = "fonts/"


--
local fonts = {}
local fontIndex = 0

--
local function enumFonts()
    local files = pd.file.listFiles(FONT_PATH)
    if not files then
        return
    end
    for i = 1, #files do
        local name = files[i]
        local font = gfx.font.new(FONT_PATH .. name)
        if font then
            fonts[#fonts + 1] = {name=name, font=font}
        end
    end
end


--
local function updateFont()
    if #fonts == 0 then
        return
    end

    fontIndex = fontIndex + 1
    if #fonts < fontIndex then
        fontIndex = 1
    end
end


--
local function drawText()
    gfx.setColor(gfx.kColorWhite)
    gfx.fillRect(0, 0, 400, 240)

    local font = fonts[fontIndex]
    if not font then
        gfx.drawText("\"" .. FONT_PATH .."\" is empty.", 10, 10)
        return
    end

    gfx.setColor(gfx.kColorBlack)
    gfx.setFont(font.font)

    TEXT_TEMPLATE[1] = "(" .. fontIndex .. "/" .. #fonts .. ") " .. TEXT_TITLE
    TEXT_TEMPLATE[2] = font.name

    local y = 10
    for i = 1, #TEXT_TEMPLATE do
        gfx.drawText(TEXT_TEMPLATE[i], 10, y)
        y = y + font.font:getHeight()
    end
end


--
local function initializeApplication()
    enumFonts()
    updateFont()
end


--
function pd.update()
    drawText()
end


--
function pd.AButtonUp()
    updateFont()
end


initializeApplication()


