from HTMLComponent import HTMLComponent
from GUIComponent import GUIComponent

from enigma import eListboxPythonStringContent, eListbox

class MenuList(HTMLComponent, GUIComponent):
	def __init__(self, list, enableWrapAround=False, content=eListboxPythonStringContent, mode=eListbox.layoutVertical, itemSize=0, itemWidth=0, itemHeight=0):
		GUIComponent.__init__(self)
		self.list = list
		self.l = content()
		self.l.setList(self.list)
		self.onSelectionChanged = [ ]
		self.enableWrapAround = enableWrapAround
		self._mode = mode
		self._itemSize = itemSize
		self._itemWidth = itemWidth
		self._itemHeight = itemHeight

	def getCurrent(self):
		return self.l.getCurrentSelection()

	GUI_WIDGET = eListbox

	def postWidgetCreate(self, instance):
		instance.setContent(self.l)
		instance.setMode(self._mode)
		if self._itemSize:
			if self._mode == eListbox.layoutVertical:
				instance.setItemHeight(self._itemSize)
			else:
				instance.setItemWidth(self._itemSize)
		if self._itemHeight and self._itemWidth and self._mode == eListbox.layoutGrid:
			instance.setItemHeight(self._itemHeight)
			instance.setItemWidth(self._itemWidth)

		self.selectionChanged_conn = instance.selectionChanged.connect(self.selectionChanged)
		self.instance.setWrapAround(self.enableWrapAround)

	def preWidgetRemove(self, instance):
		instance.setContent(None)
		self.selectionChanged_conn = None

	def selectionChanged(self):
		for f in self.onSelectionChanged:
			f()

	def getSelectionIndex(self):
		return self.l.getCurrentSelectionIndex()

	def getSelectedIndex(self):
		return self.l.getCurrentSelectionIndex()

	def setList(self, list):
		self.list = list
		self.l.setList(self.list)

	def moveToIndex(self, idx):
		if self.instance is not None:
			self.instance.moveSelectionTo(idx)

	def pageUp(self):
		if self.instance is not None:
			self.instance.moveSelection(self.instance.pageUp)

	def pageDown(self):
		if self.instance is not None:
			self.instance.moveSelection(self.instance.pageDown)

	def up(self):
		if self.instance is not None:
			self.instance.moveSelection(self.instance.moveUp)

	def down(self):
		if self.instance is not None:
			self.instance.moveSelection(self.instance.moveDown)

	def selectionEnabled(self, enabled):
		if self.instance is not None:
			self.instance.setSelectionEnable(enabled)
