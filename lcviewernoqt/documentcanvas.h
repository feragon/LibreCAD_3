#pragma once

#include <functional>

#include "painters/lcpainter.h"

#include "cad/dochelpers/entitycontainer.h"
#include "drawitems/lcvdrawitem.h"
#include "events/drawevent.h"
#include <cad/base/cadentity.h>

#include <cad/events/addentityevent.h>
#include <cad/events/commitprocessevent.h>
#include <cad/events/removeentityevent.h>
#include <nano-signal-slot/nano_signal_slot.hpp>

#include <cad/document/document.h>

// Minimum linewidth we reander, below this the lines might start to loog 'jagged'
// We migth one to consider at lower linewidth to simply reduce alpha to get a similat effect of smaller line?
static const double MINIMUM_READER_LINEWIDTH = 1.0;
namespace LCViewer {

class DocumentCanvas : public std::enable_shared_from_this<DocumentCanvas> {
    public:
        DocumentCanvas(std::shared_ptr<lc::Document> document,
                       const std::function<LcPainter *(const unsigned int, const unsigned int)>& createPainterFunctor = nullptr,
                       const std::function<void(LcPainter*)>& deletePainterFunctor = nullptr
        );

        virtual ~DocumentCanvas();

        /**
         * @brief render
         * this document using it's painter's
         * The provided wPainter will allow to paint each item on the final device
         * @param wPainter
         */
        void render(std::function<void(LcPainter&)> before, std::function<void(LcPainter&)> after);

        /**
         * @brief drawEntity
         * Draw entity without adding it to the current document
         * @param entity LCVDrawItem_CSPtr
         * @param insert Insert entity if we are rendering a bloc
         */
        void drawEntity(LCVDrawItem_CSPtr entity, lc::entity::Insert_CSPtr insert = nullptr);

        /**
         * @brief autoScale
         * Found the bounds of the current document and scale into this and center on screen
         */
        void autoScale();

        /**
         * @brief Set display area
         * @param area Area to display
         */
        void setDisplayArea(const lc::geo::Area& area);

        void pan(double move_x, double move_y);

        void transX(int x);

        void transY(int y);
        /**
         * @brief zoom
         * into a specific area
         * @param factor
         * @param deviceCenterX
         * @param deviceCenterY
         */
        void zoom(double factor, bool relativezoom, unsigned int deviceCenterX, unsigned int deviceCenterY);

        /**
         * @brief zoom
         * into a specific area with a zoom factor
         * Ensures that the specified user location is located at the device location
         * on a specific zoom factor
         * @param factor
         * @param userCenterX
         * @param userCenterY
         * @param deviceCenterX
         * @param deviceCenterY
         */
        void zoom(double factor, bool relativezoom, double userCenterX, double userCenterY, unsigned int deviceCenterX, unsigned int deviceCenterY);

        /**
         * @brief newSize
         * for the device. When using a pixel based device this is the number of pixels of the painter
         * @param width
         * @param hight
         */
        void newDeviceSize(unsigned int width, unsigned int hight);

        /**
         * @brief bounds
         * return the opproximate size of the current document
         * @return
         */
        lc::geo::Area bounds() const;

        /**
         * @brief makeSelection
         * within the document. It wil color the area red/green depending on the occupies flag.
         * The coordinates must be given in user coordinates
         * @param x
         * @param y
         * @param w
         * @param y
         * @param occupies
         * @param addTo -- when set to true, we add it to the current selection
         */
        void makeSelection(double x, double y, double w, double h, bool occupies, bool addTo = false);

        /**
         * @brief makeSelectionDevice
         * based on device coordinate rather then user coordinates
         * @param x
         * @param y
         * @param w
         * @param h
         * @param occupies
         * @param addTo -- when set to true, we add it to the current selection
         */
        void makeSelectionDevice(unsigned int x, unsigned int y, unsigned int w, unsigned int h, bool occupies, bool addTo = false);

        /**
        * @brief closeSelection
        * appends current selection to document
        */
        void closeSelection();

        /**
        * @brief removeSelectionArea removed the colord selection area from the screen
        */
        void removeSelectionArea();

        void removeSelection();

        lc::EntityContainer<lc::entity::CADEntity_SPtr> selection();

        /**
         *
         */
        void setPositionDevice(int x, int y);



        Nano::Signal<void(DrawEvent const & drawEvent)> & background () ;
        Nano::Signal<void(DrawEvent const & drawEvent)> & foreground () ;

        /**
         * I don't like this because it requires a painting context
         * However, for now I will do it because there wasn't a easy and quick way to have
         * some matrix calculations done.
         */
        void device_to_user(double *x, double *y) const {
            if (_painter) {
                _painter->device_to_user(x, y);
            }
        }

        /**
         * Return the underlaying document
         */
        std::shared_ptr<lc::Document> document() const;

        /**
         * Get the current entity container,
         * don not store this as a reference, always call it
         */
        const lc::EntityContainer<lc::entity::CADEntity_SPtr>& entityContainer() const;

        /*
         * Return CADEntity as LCVDrawItem
         */
        static LCVDrawItem_SPtr asDrawable(lc::entity::CADEntity_CSPtr entity);
    private:
        LcPainter* _painter;

        void on_addEntityEvent(const lc::AddEntityEvent&);
        void on_removeEntityEvent(const lc::RemoveEntityEvent&);
        void on_commitProcessEvent(const lc::CommitProcessEvent&);

    private:
        double drawWidth(lc::entity::CADEntity_CSPtr entity, lc::entity::Insert_CSPtr insert);
        std::vector<double> drawLinePattern(
                lc::entity::CADEntity_CSPtr entity,
                lc::entity::Insert_CSPtr insert,
                double width
        );
        lc::Color drawColor(lc::entity::CADEntity_CSPtr entity, lc::entity::Insert_CSPtr insert, bool selected);

        // Original document
        std::shared_ptr<lc::Document> _document;

        // Local entity container
        lc::EntityContainer<lc::entity::CADEntity_SPtr> _entityContainer;

        Nano::Signal<void(DrawEvent const & event)> _background;
        Nano::Signal<void(DrawEvent const & event)> _foreground;



        // Two functor's that allow creation and destruction of painters
        std::function<LcPainter *(const unsigned int, const unsigned int)> _createPainterFunctor;
        std::function<void(LcPainter*)> _deletePainterFunctor;

        // Maximum and minimum allowed scale factors
        double _zoomMin;
        double _zoomMax;

        // Current's device width and height
        unsigned int _deviceWidth;
        unsigned int _deviceHeight;

        // When !=null it show's a selected area
        lc::geo::Area* _selectedArea;

        // When set to true, a entity will be selected if it intersects or occupies,
        // when false it will only select when the entity is fully contained
        bool _selectedAreaIntersects;

        // Functor to draw a selected area, that's the green or read area...
        std::function<void(LcPainter&, lc::geo::Area, bool)> _selectedAreaPainter;

        lc::EntityContainer<lc::entity::CADEntity_SPtr> _selectedEntities;
        lc::EntityContainer<lc::entity::CADEntity_SPtr> _newSelection;
};

DECLARE_SHORT_SHARED_PTR(DocumentCanvas)
}
