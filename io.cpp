#include "io.h"
#include "encript.h"

Io::Io(const QString& fileName, uint64_t passwd) : file(fileName), key(passwd) {
	file.open(QIODevice::ReadWrite);
	if (QFile::exists(fileName)) { // load header
		QDataStream ds(&file);
		ds >> entries;
	}
}

void Io::write_block(idx_t idx, const QByteArray& data) {
	assert(data.size() <= block_size);
	file.seek(header_size + idx * block_size);
	file.write(data);
}

idx_t Io::new_block(const QByteArray& data) {
	if (allocatedBlocks.empty()) // first init
		for (auto& entry : entries)
			for (idx_t idx : entry)
				allocatedBlocks.insert(idx);
	// find a free block
	idx_t i = 0, res;
	auto it = allocatedBlocks.begin();
	for (; it != allocatedBlocks.end(); ++it, ++i)
		if (*it > i) {
			res = *it; break;
		}
	if (it == allocatedBlocks.end()) {
		allocatedBlocks.insert(i);
		res = i;
	}
	// write data
	write_block(res, data);
	return res;
}

void Io::write_header() {
	QDataStream ds(&file);
	file.seek(0);
	ds << entries;
}

Io::~Io() {
	write_header();
	file.close();
}

QString Io::read_block(idx_t idx, idx_t size) const {
	auto f = const_cast<QFile*>(&file);
	f->seek(header_size + idx * block_size);
	QByteArray data = f->read(size);
	return decript(data, key);
}

QString Io::get_entry(idx_t idx) const {
	QString res;
	for (idx_t i : entries[idx])
		res.append(read_block(i));
	return res;
}

void Io::save_entry(const QString& text, idx_t idx) {
	QByteArray data = encript(text, key);
	idx_t requiredBlocks = (data.size() + block_size - 1) / block_size;
	if (idx == (idx_t)-1) {
		idx = entries.size();
		entries.push_back({});
	}
	auto& entry = entries[idx];
	while (requiredBlocks < entry.size()) {
		allocatedBlocks.remove(entry.back());
		entry.pop_back();
	}
	idx_t blockIdx = 0;
	for (; blockIdx < entry.size(); ++blockIdx)
		write_block(entry[blockIdx], data.mid(blockIdx * block_size, block_size));
	for (; requiredBlocks > entry.size(); ++blockIdx)
		entry.push_back(new_block(data.mid(blockIdx * block_size, block_size)));
}

QStringList Io::get_mata(int previewSize) const {
	QStringList strl;
	for (auto& entry : entries) {
		assert(entry.size());
		strl.push_back(read_block(entry[0], previewSize));
	}
	return strl;
}
