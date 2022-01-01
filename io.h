#ifndef IO_H
#define IO_H
#include <QString>
#include <QFile>
#include <QVector>
#include <QDataStream>
#include <QSet>

/*** The structure of storage:
 * header occupies the first 16kB (supports up to 2M words);
 * remaining space are split into blocks of 1kB which can store 512 words;
 * every entry occupies a few blocks specified in the header.
 *
 *** The structure of header:
 * entry_num: uint16;
 * list of entries:
 *   block_num: uint16 (supports up to 30M words);
 *   list of blocks it occupies: uint16.
*/
typedef quint16 idx_t;

class Io {
	uint64_t key;
	QFile file;
	QVector<QVector<idx_t>> entries;
	QSet<idx_t> allocatedBlocks; // lazy init until first save
	// modify header_size to 256k will make it support up tp 30M words
	static constexpr quint64 header_size = 16 * 1024, block_size = 1024;
	QString read_block(idx_t idx, idx_t size = block_size) const;
	void write_block(idx_t idx, const QByteArray& data);
	idx_t new_block(const QByteArray& data);
	void write_header();
public:
	Io(const QString& fileName, uint64_t passwd);
	~Io();
	// encript & decript in this process
	QString get_entry(idx_t idx) const;
	QStringList get_mata(int previewSize = 32) const;
	void save_entry(const QString& text, idx_t idx = -1);
};

#endif
