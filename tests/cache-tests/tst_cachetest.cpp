#include <QtTest>

#include "../../src/http/cache/cacheditem.h"

struct TestRemoteResourceInfo : RemoteResourceInfo
{
public:
	TestRemoteResourceInfo(QString mime, size_t size, bool &isDeleted): RemoteResourceInfo(mime, size), _isDeleted(isDeleted) { }
	virtual ~TestRemoteResourceInfo(){ _isDeleted = true; }

private:
	bool& _isDeleted;
};

class CacheTest : public QObject
{
	Q_OBJECT

public:
	CacheTest() {}
	~CacheTest() {}

private slots:
	void cacheInfo();

	void isDataCachedFully();

	void isDataCached_data();
	void isDataCached();

	void dataFull();
	void data();

	void cacheDataOne();
	void cacheDataTwo();
	void cacheDataTwoIntersecting();
	void cacheDataTwoWithGap();

	void usedMemorySize();
	void remove();
};

void CacheTest::cacheInfo()
{
	bool isDeleted = false;

	CachedItem *item = new CachedItem();

	{
		QSharedPointer<RemoteResourceInfo> info(new TestRemoteResourceInfo("text/html", 200, isDeleted));
		item->cacheInfo(info);
	}

	QVERIFY(item->info() != nullptr);

	delete item;

	QVERIFY(isDeleted);
}

void CacheTest::isDataCachedFully()
{
	QByteArray data("12345");
	CachedItem *item = new CachedItem();
	item->cacheData(0, data);

	QVERIFY2(!item->isDataCachedFully(), "Data without info is not assumed as 'cached fully'.");

	item->cacheInfo(QSharedPointer<RemoteResourceInfo>(new RemoteResourceInfo("text/numbers", data.count())));

	QVERIFY(item->isDataCachedFully());

	item->cacheInfo(QSharedPointer<RemoteResourceInfo>(new RemoteResourceInfo("text/numbers", data.count() + 2)));

	QVERIFY(!item->isDataCachedFully());

	delete item;
}

void CacheTest::isDataCached_data()
{
	// Lets assume there is a content "12346789", however only "234" is cached.
	// Full data: 123456789
	// Cached   : _234____9

	QTest::addColumn<int>("offset");
	QTest::addColumn<int>("size");
	QTest::addColumn<bool>("isCached");

	QTest::newRow("not cached - left") << 0 << 1 << false;
	QTest::newRow("not cached - left touching") << 1 << 1 << false;
	QTest::newRow("partly cached - left") << 1 << 2 << false;

	QTest::newRow("cached - left") << 2 << 1 << true;
	QTest::newRow("cached - full") << 2 << 3 << true;
	QTest::newRow("cached - middle") << 3 << 1 << true;
	QTest::newRow("cached - middle right") << 3 << 2 << true;
	QTest::newRow("cached - right") << 4 << 1 << true;

	QTest::newRow("partly cached - right") << 4 << 2 << false;
	QTest::newRow("not cached - right - touching") << 5 << 1 << false;
	QTest::newRow("not cached - right") << 6 << 2 << false;
	QTest::newRow("not cached - right with hole") << 6 << 3 << false;
}

void CacheTest::isDataCached()
{
	CachedItem *item = new CachedItem();
	item->cacheData(2, QByteArray("234"));

	QFETCH(int, offset);
	QFETCH(int, size);
	QFETCH(bool, isCached);

	QCOMPARE(item->isDataCached(offset, size), isCached);

	delete item;
}

void CacheTest::dataFull()
{
	QByteArray data("12345");
	CachedItem *item = new CachedItem();

	item->cacheData(0, data);
	item->cacheInfo(QSharedPointer<RemoteResourceInfo>(new RemoteResourceInfo("text/numbers", data.count())));

	QCOMPARE(item->dataFull(), data);
}

void CacheTest::data()
{
	//Content: 12345sdfghi

	CachedItem *item = new CachedItem();
	QByteArray data1("12345");
	QByteArray data2("asdfghi");

	item->cacheData(0, data1);
	item->cacheData(4, data2);

	item->cacheInfo(QSharedPointer<RemoteResourceInfo>(new RemoteResourceInfo("text/numbers", 8)));

	QCOMPARE(item->data(0, 4), QByteArray("1234"));
	QCOMPARE(item->data(3, 4), QByteArray("45sd"));
	QCOMPARE(item->data(6, 2), QByteArray("df"));

	QCOMPARE(item->data(0, 1000), QByteArray("12345sdf"));
}

void CacheTest::cacheDataOne()
{
	//Content: __12345__

	CachedItem *item = new CachedItem();
	QByteArray data1("12345");

	item->cacheData(2, data1);

	QCOMPARE(item->data(2, 2), QByteArray("12"));
}

void CacheTest::cacheDataTwo()
{
	//Piece one: __123____
	//Piece two: ______56_
	//Content:   __123_56_

	CachedItem *item = new CachedItem();
	QByteArray data1("123");
	QByteArray data2("56");

	item->cacheData(2, data1);
	item->cacheData(6, data2);

	QVERIFY(item->isDataCached(2, 6) == false);
	QCOMPARE(item->data(2, 3), QByteArray("123"));
	QCOMPARE(item->data(6, 2), QByteArray("56"));
	QCOMPARE(item->data(2, 1000), QByteArray("12356"));
}

void CacheTest::cacheDataTwoIntersecting()
{
	//Piece one  : __123____
	//Piece two  : ____asdf_
	//Piece three: ______yz_
	//Content:     __123sdf_

	CachedItem *item = new CachedItem();
	QByteArray data1("123");
	QByteArray data2("asdf");
	QByteArray data3("yz");

	item->cacheData(2, data1);
	item->cacheData(4, data2);
	item->cacheData(6, data3);

	QVERIFY(item->isDataCached(2, 6) == true);
	QCOMPARE(item->data(2, 3), QByteArray("123"));
	QCOMPARE(item->data(6, 2), QByteArray("df"));
	QCOMPARE(item->data(2, 1000), QByteArray("123sdf"));
}

void CacheTest::cacheDataTwoWithGap()
{
	//Piece one  : __1______
	//Piece two  : _____45__
	//Piece three: _asdfghy_
	//Content:     _a1df45y_

	CachedItem *item = new CachedItem();
	item->cacheData(2, QByteArray("1"));
	item->cacheData(5, QByteArray("45"));
	item->cacheData(1, QByteArray("asdfghy"));

	QCOMPARE(item->data(1, 3), QByteArray("a1d"));
	QCOMPARE(item->data(6, 2), QByteArray("5y"));
	QCOMPARE(item->data(1, 1000), QByteArray("a1df45y"));
	QVERIFY(item->isDataCached(2, 6) == true);
}

void CacheTest::usedMemorySize()
{
	//Piece one  : __1______
	//Piece two  : _____45__
	//Piece three: _asdfghy_
	//Content:     _a1df45y_

	CachedItem *item = new CachedItem();

	item->cacheData(2, QByteArray("1"));
	item->cacheData(5, QByteArray("45"));
	item->cacheData(1, QByteArray("asdfghy"));

	QCOMPARE(item->usedMemorySize(), 7);
}

void CacheTest::remove()
{
	//Piece one  : __1______
	//Piece two  : _____45__
	//Piece three: _asdfghy_
	//Content:     _a1df45y_

	CachedItem *item = new CachedItem();

	item->cacheData(2, QByteArray("1"));
	item->cacheData(5, QByteArray("45"));
	item->cacheData(1, QByteArray("asdfghy"));

	int freedMemorySize = item->remove(2);

	QVERIFY(freedMemorySize >= 2);
	QCOMPARE(item->usedMemorySize(), 7 - freedMemorySize);
}

QTEST_APPLESS_MAIN(CacheTest)

#include "tst_cachetest.moc"
