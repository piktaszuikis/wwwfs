#ifndef HUMANREADABLECONTENTRENDERER_H
#define HUMANREADABLECONTENTRENDERER_H

#include "rendererbase.h"

class HumanReadableRenderer : public RendererBase
{
public:
	HumanReadableRenderer(ContentContainer* container, Folder *parent);

	// RendererBase interface
protected:
	bool isFolder(HttpItem *item) override;
	ContentItem *createFile(HttpItem *item) override;
	ContentItem *createFolder(HttpItem *item) override;
	ContentItem *createSymlink(HttpItem *item, ContentItem *existing) override;

private:
	QString getName(HttpItem *item);
};

#endif // HUMANREADABLECONTENTRENDERER_H
