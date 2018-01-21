#include "humanreadablerenderer.h"

HumanReadableRenderer::HumanReadableRenderer(ContentContainer *container, Folder *parent)
	:RendererBase(container, parent)
{

}


bool HumanReadableRenderer::isFolder(HttpItem *item)
{
	return item->getHtmlTag() == "a";
}

ContentItem *HumanReadableRenderer::createFile(HttpItem *item)
{
	//getting size should probably be done in async manier...

	return new File(nextID(), getName(item), item->getSize(), item->getUrl());
}

ContentItem *HumanReadableRenderer::createFolder(HttpItem *item)
{
	return new Folder(nextID(), getName(item), item->getUrl());
}

QString HumanReadableRenderer::getName(HttpItem *item)
{
	if(!item->getTitle().isEmpty())
		return getUniqueName(item->getTitle());

	if(!item->getAlt().isEmpty())
		return getUniqueName(item->getAlt());

	QString name = item->getUrl().fileName();
	if(!name.isEmpty())
		return getUniqueName(name);

	name = item->getUrl().path();
	if(name.endsWith('/'))
		name.remove(1, name.length() - 1);

	if(name.contains('/'))
		name = name.mid(name.lastIndexOf('/') + 1);

	if(!name.isEmpty())
		return getUniqueName(name);

	return getUniqueName("empty");
}
