import "CoreLibs/object"
import "CoreLibs/graphics"
import "CoreLibs/timer"

local pd = playdate
local gfx = pd.graphics


--
local TEXT_TITLE = "- PDFontTool TEST -"
local TEXT_TEMPLATE =
{
    "",
    "",
    "天の原ふりさけみれば春日なる",
    "三笠の山に出でし月かも 安倍仲麿"
}
local FONT_PATH = "fonts/"


--
local _fontDetails = {}
local _nowFontIndex = 0
local _nowFont = nil
local _nowLoading = false


--
local function enumFonts()
    local files = pd.file.listFiles(FONT_PATH)
    if not files then
        return
    end
    for i = 1, #files do
        local name = files[i]
        local findExt = string.find(name, '%.')
        if not findExt then
            goto continue
        end

        local ext = string.sub(name, findExt + 1, #name)
        if ext == "pft" then
            --print("Font found " .. #_fontDetails + 1 .. " : " .. name)
            _fontDetails[#_fontDetails + 1] = {name=name, path=FONT_PATH .. name}
        end

        ::continue::
    end
end


--
local function updateFont()
    if #_fontDetails == 0 then
        return
    end

    _nowFontIndex = _nowFontIndex + 1
    if #_fontDetails < _nowFontIndex then
        _nowFontIndex = 1
    end

    _nowLoading = true
    pd.timer.performAfterDelay(1, function ()
        _nowFont = gfx.font.new(_fontDetails[_nowFontIndex].path)
        _nowLoading = false
    end)
end


--
local function drawText()
    gfx.setColor(gfx.kColorWhite)
    gfx.fillRect(0, 0, 400, 240)

    if _nowLoading then
        gfx.drawText("Loading ...", 10, 10)
        return
    end

    local detail = _fontDetails[_nowFontIndex]
    if not detail then
        return
    end

    if not _nowFont then
        gfx.drawText("\"" .. detail.name .."\" is empty.", 10, 10)
        return
    end

    gfx.setColor(gfx.kColorBlack)
    gfx.setFont(_nowFont)

    TEXT_TEMPLATE[1] = "(" .. _nowFontIndex .. "/" .. #_fontDetails .. ") " .. TEXT_TITLE
    TEXT_TEMPLATE[2] = detail.name

    local y = 10
    for i = 1, #TEXT_TEMPLATE do
        gfx.drawText(TEXT_TEMPLATE[i], 10, y)
        y = y + _nowFont:getHeight()
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
    pd.timer.updateTimers()
end


--
function pd.AButtonUp()
    updateFont()
end


initializeApplication()


