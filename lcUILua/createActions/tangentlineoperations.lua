TangentLineOperations = {}
TangentLineOperations.__index = TangentLineOperations

setmetatable(TangentLineOperations, {
    __index = CreateOperations,
    __call = function (o, ...)
        local self = setmetatable({}, o)
        self:_init(...)
        return self
    end,
})

function TangentLineOperations:_init(id)
    self.lastPoint = nil
    self.entity_id = ID():id()
    message("Click on first point", id)

    self._id = id

    CreateOperations._init(self, id)
    luaInterface:registerEvent('selectionChanged', self)
end

function TangentLineOperations:onEvent(eventName, data)
    if(Operations.forMe(self, data) == false) then
        return
    end

    if(eventName == "point") then
        self:newPoint(data["position"])
    elseif(eventName == "selectionChanged") then
        self:chooseCircle()
    end
end

function TangentLineOperations:newPoint(point)
    if(self.lastPoint == nil) then
        self.lastPoint = point
        message("Click on target circle", self._id)
    end
end

function TangentLineOperations:chooseCircle()
    local selection = getWindow(self._id):selection()
    if(#selection == 0) then
        message("0 items selected", self._id)
        return
    end

    if(self.lastPoint == nil) then
        message("Select fp before", self._id)
        return
    end

    local points = Tangent.lineTangent(selection[1], self.lastPoint) -- needs better handling of [1]

    local layer = active_layer(self.target_widget)
    local metaInfo = active_metaInfo(self.target_widget)
    local b = EntityBuilder(getWindow(self.target_widget):document())

    for k, point in pairs(points) do
        local l = Line(self.lastPoint, point, layer, metaInfo)
        self:createEntity(l)
    end
end

function TangentLineOperations:createTangentLine(p1, p2)
    local layer = active_layer(self.target_widget)
    local metaInfo = active_metaInfo(self.target_widget)

    if(self.length ~= nil) then
        local angle = p1:angleTo(p2)
        local relativeCoordinate = Coordinate._fromAngle(angle):mulDouble(self.length)
        p2 = p1:add(relativeCoordinate)
    end

    local l = TangentLine(p1, p2, layer, metaInfo)
    l:setId(self.entity_id)

    return l
end
