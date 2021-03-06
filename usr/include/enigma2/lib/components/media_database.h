#ifndef __lib_components_mediadatabase_h_
#define __lib_components_mediadatabase_h_

#include <lib/base/message.h>
#include <lib/base/stl_types.h>
#include <lib/base/thread.h>
#include <lib/components/file_monitor.h>
#include <lib/components/media_scanner.h>
#include <lib/gdi/esize.h>

#include <QSqlQuery>
#include <QSqlDatabase>

class gPixmap;

#ifndef SWIG
struct query_data
{
	std::string query_string;
	file_metadata result;
};

struct insert_result
{
	long long id;
	bool created;
	bool error;
	bool skipped;
	insert_result(long long id = 0, bool created = true, bool error = false, bool skipped = false) :
			id(id), created(created), error(error), skipped(skipped)
	{
	}
};
#endif

class eMediaDatabaseHandler;

class eMediaDatabaseResult : public iObject
{
	SWIG_AUTODOC
	DECLARE_REF(eMediaDatabaseResult)
	bool m_error;
	int m_rowsAffected;
	int m_lastInsertId;
	std::string m_errorDriverText;
	std::string m_errorDatabaseText;
	stringMapVector m_data;
public:
#ifdef SWIG
	eMediaDatabaseResult();
	~eMediaDatabaseResult();
#endif
	bool error(){ return m_error; };
	int rowsAffected(){ return m_rowsAffected; };
	int lastInsertId(){ return m_lastInsertId; };
	const std::string &errorDriverText(){ return m_errorDriverText; };
	const std::string &errorDatabaseText(){ return m_errorDatabaseText; };
	const stringMapVector &data(){ return m_data; };
#ifndef SWIG
	void setError(bool error){ m_error = error; };
	void setRowsAffected(int rowsAffected){ m_rowsAffected = rowsAffected; };
	void setLastInsertId(int lastInsertId){ m_lastInsertId = lastInsertId; };
	void setErrorDriverText( const std::string &errorDriverText ){ m_errorDriverText = errorDriverText; };
	void setErrorDatabaseText( const std::string &errorDatabaseText){ m_errorDatabaseText = errorDatabaseText; };
	stringMapVector *dataPtr(){ return &m_data; };
#endif

};

class eMediaDatabase : public sigc::trackable
{
	SWIG_AUTODOC
	static eMediaDatabase *instance;
	eMediaScanner *m_scanner;
	eMediaDatabaseHandler *m_db;
	std::list<eFileWatch*> m_watches;
	std::map<eFileWatch*, sigc::connection> m_watchconns;
	typedef void (eMediaDatabase::*table_line_extension_func)(stringMap *item, QSqlQuery*);

#ifdef SWIG
	eMediaDatabase();
	~eMediaDatabase();
#endif

public:
	static const char FIELD_ID[];
	static const char FIELD_FILE_ID[];
	static const char FIELD_DIR_ID[];
	static const char FIELD_ALBUM_ID[];
	static const char FIELD_ARTIST_ID[];
	static const char FIELD_ALBUM_ARTIST_ID[];
	static const char FIELD_GENRE_ID[];
	static const char FIELD_CODEC_ID[];
	static const char FIELD_PATH[];
	static const char FIELD_FILENAME[];
	static const char FIELD_SIZE[];
	static const char FIELD_DURATION[];
	static const char FIELD_POPULARITY[];
	static const char FIELD_LASTPLAYPOS[];
	static const char FIELD_LASTPLAYED[];
	static const char FIELD_LASTMODIFIED[];
	static const char FIELD_LASTUPDATED[];
	static const char FIELD_TITLE[];
	static const char FIELD_TRACK[];
	static const char FIELD_TRACKS_TOTAL[];
	static const char FIELD_DATE[];
	static const char FIELD_COMMENT[];
	static const char FIELD_ARTIST[];
	static const char FIELD_ALBUM_ARTIST[];
	static const char FIELD_ALBUM[];
	static const char FIELD_GENRE[];
	static const char FIELD_CODEC[];
	static const char FIELD_CODEC_LONG[];
	static const char FIELD_WIDTH[];
	static const char FIELD_HEIGHT[];
	static const char FIELD_FRAMERATE[];
	static const char FIELD_HD[];
	static const char FIELD_WIDESCREEN[];
	static const char FIELD_RECORDING[];
	static const char FIELD_PLAYLIST_NAME[];
	static const char FIELD_FILE_URI[];
	static const char FIELD_POS[];
	static const char FIELD_DESCRIPTION[];
	static const char FIELD_SERVICEREFRENCE[];
	static const char FIELD_TYPE[];
	static const char FIELD_NAME[];
	static const char FIELD_COVER_ART_ID[];
	static const char FIELD_SAVED_AUDIO_TRACK[];
	static const char FIELD_SAVED_SUBTITLE_TRACK[];
	static const char FIELD_TAG_ID[];
	static const char FIELD_TAG[];
	static const char FIELD_CREATED[];

	static eMediaDatabase *getInstance();

#ifndef SWIG
	eMediaDatabase();
	~eMediaDatabase();

	eMediaDatabaseHandler *getDatabaseHandler();

	void addToDatabase(const std::list<file_metadata> &data);
	bool _dbAddAudio(const file_data &fmd, const audio_track &at, const audio_metadata &amd);
	bool _dbAddVideo(const file_data &fmd, const video_data &vt);

	void onScanStatistics(const std::string &dir, uint64_t total, uint64_t successful, uint64_t skipped);
	void onScanFinished(const std::string &dir, uint64_t total, uint64_t successful, uint64_t skipped);
	void onInsertFinished(uint64_t success, uint64_t skipped, uint64_t errors, const std::list<int> &ids);

	ePtr<eMediaDatabaseResult> resultFromQuery(QSqlQuery* qry, table_line_extension_func extendItem=0);
	void extendPlaylistItemAttributes(stringMap *item, QSqlQuery *qry);

	void onProcessEvents(bool finished);
	void onFileChanged(eFileWatch *watch, eFileEvent event);

#endif
	bool resetDatabase();

	void getCurrentScanPath(std::string &SWIG_OUTPUT);
	void getEnqueuedPaths(std::list<std::string> &SWIG_OUTPUT);

	void addPath(const std::string &path, bool watch=false);
	void rescanPath(const std::string &path, bool isRecording=false);
	bool requestScanStatistics();
	ePtr<eMediaDatabaseResult> setParentDirectoryWatched(int dir_id, bool watched);
	ePtr<eMediaDatabaseResult> deleteParentDirectory(int dir_id);
	ePtr<eMediaDatabaseResult> getParentDirectories();
	ePtr<eMediaDatabaseResult> getAllDirectories();

	ePtr<eMediaDatabaseResult> filterAudio(const std::string &needle, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> filterByAlbum(const std::string &album, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> filterByArtistAlbum(const std::string &artist, const std::string &album, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> filterByAlbumArtistAlbum(const std::string &artist, const std::string &album, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> filterByArtist(const std::string &artist, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> filterByAlbumArtist(const std::string &artist, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> filterByGenre(const std::string &genre, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> filterByTitle(const std::string &title, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getAllArtists(int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getAllAlbumArtists(int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getArtists(const std::string &artist, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getAllAlbums(int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getAlbums(const std::string &album, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getAlbumsByArtist(const std::string &artist, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getAlbumsByAlbumArtist(const std::string &album_artist, int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getTracksByAlbumId(int album_id);
	ePtr<eMediaDatabaseResult> getAllAudio(int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getAllVideos(int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getVideos(const std::string &dir, const std::list<std::string> &tags = std::list<std::string>(), int limit = -1, int offset = 0);

	ePtr<eMediaDatabaseResult> addRecording(const std::string &filepath, const std::string & title, int created);
	ePtr<eMediaDatabaseResult> getAllRecordings(int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getRecordings(const std::string &dir, const std::list<std::string> &tags = std::list<std::string>(), int limit = -1, int offset = 0);
	ePtr<eMediaDatabaseResult> getRecordMeta(int file_id);
	ePtr<eMediaDatabaseResult> setRecordMeta(int file_id, const std::string & ref, const std::string &name, const std::string &description, const std::string &service_data, int64_t duration, int64_t filesize, int lastmodified, int created);
	ePtr<eMediaDatabaseResult> getRecordEit(int file_id);
	ePtr<eMediaDatabaseResult> setRecordEit(int file_id, uint8_t eit_raw[], bool update=false);
	ePtr<eMediaDatabaseResult> getRecordTags(int file_id);
	ePtr<eMediaDatabaseResult> setRecordTags(int file_id, const std::vector<std::string> &tags);
	ePtr<eMediaDatabaseResult> setLastPlayed(int file_id, int64_t lastplayed, int64_t lastplaypos);
	SWIG_VOID(int) getLastPlayed(int file_id, int64_t &lastplayed, int64_t &lastplaypos);

	ePtr<eMediaDatabaseResult> getFileByPath(const std::string &filepath);

	ePtr<eMediaDatabaseResult> setFileAttribute(int file_id, const std::string &key, const std::string &value);
	ePtr<eMediaDatabaseResult> deleteFileAttribute(int file_id, const std::string &key);
	ePtr<eMediaDatabaseResult> getFileAttributes(int file_id);
	ePtr<eMediaDatabaseResult> getFileAttribute(int file_id, const std::string &key);
	SWIG_VOID(int) getFileAttributeValue(int file_id, const std::string &key, std::string &SWIG_OUTPUT);

	SWIG_VOID(int) getCoverArt(int cover_art_id, ePtr<gPixmap> &SWIG_OUTPUT, const eSize &size=eSize(), const eSize &aspect=eSize());
	ePtr<eMediaDatabaseResult> getCoverArtData(int cover_art_id);
	ePtr<eMediaDatabaseResult> getCoverArtData(const std::string &cover_art_key);

	SWIG_VOID(int) getAlbumCoverArt(int cover_art_id, ePtr<gPixmap> &SWIG_OUTPUT, const eSize &size=eSize(), const eSize &aspect=eSize());
	ePtr<eMediaDatabaseResult> getAlbumCoverArtData(int album_id);
	ePtr<eMediaDatabaseResult> getAlbumCoverArtId(int album_id);

	ePtr<eMediaDatabaseResult> addPlaylist(const std::string &name, int type);
	ePtr<eMediaDatabaseResult> getPlaylist(int id);
	ePtr<eMediaDatabaseResult> getPlaylistByName(const std::string &name, int type);
	ePtr<eMediaDatabaseResult> getPlaylistItemsById(int id);
	ePtr<eMediaDatabaseResult> getPlaylistItemsByName(const std::string &name, int type=-1);
	ePtr<eMediaDatabaseResult> getPlaylists(int type=-1);
	ePtr<eMediaDatabaseResult> addToPlaylistById(int playlist_id, int file_id, int pos);
	ePtr<eMediaDatabaseResult> addToPlaylistByUri(int playlist_id, const std::string &uri, int pos, const stringMap &attributes);
	ePtr<eMediaDatabaseResult> updatePlaylistItem(int playlist_id, stringMap item);
	ePtr<eMediaDatabaseResult> clearPlaylist(int playlist_id);
	ePtr<eMediaDatabaseResult> savePlaylist(int playlist_id, const std::string &name, int type, const stringMapVector &items);

	ePtr<eMediaDatabaseResult> removeFile(int file_id);
	ePtr<eMediaDatabaseResult> query(const std::string &sql, const stringList &values=stringList(), bool rw = false);

	/* void scanStatistics( std::string, uint64_t, uint64_t, uint64_t ); */
	eSignal4<void, std::string, uint64_t, uint64_t, uint64_t> scanStatistics;
	/* void scanFinished( std::string, uint64_t, uint64_t, uint64_t ); */
	eSignal4<void, std::string, uint64_t, uint64_t, uint64_t> scanFinished;
	/* void insertFinished(uint64_t, uint64_t, uint64_t, PyObject*); */
	eSignal4<void, uint64_t, uint64_t, uint64_t, intList > insertFinished;
	/* void priorityInsertFinished ( char*, uint64_t, uint64_t, uint64_t, int ); */
	eSignal5<void, const char *, uint64_t, uint64_t, uint64_t, int> priorityInsertFinished;
};

SWIG_TEMPLATE_TYPEDEF(ePtr<eMediaDatabaseResult>, eMediaDatabaseResultPtr);

#ifndef SWIG

class eVersionedDatabase {
public:
	virtual ~eVersionedDatabase(){};
protected:
	void checkVersion(const QSqlDatabase &db_rw, int requiredVersion);
	virtual bool upgradeSchema(int from)=0;
};

class eMediaDatabaseHandler : public eVersionedDatabase, public eMainloop_native, public eThread, public sigc::trackable
{
	int SCHEMA_VERSION = 8;

	std::string normalizePath(const std::string &path);
	bool upgradeSchema(int from);
	bool upgradeSchema0_to_1();
	bool upgradeSchema1_to_2();
	bool upgradeSchema2_to_3();
	bool upgradeSchema3_to_4();
	bool upgradeSchema4_to_5();
	bool upgradeSchema5_to_6();
	bool upgradeSchema6_to_7();
	bool upgradeSchema7_to_8();

public:
	eMediaDatabaseHandler();
	~eMediaDatabaseHandler();

	static pthread_mutex_t priority_files_lock, files_lock;

	void insertList(const std::string &dir, std::list<file_metadata> &files, std::map< uint32_t, ePtr<cover_art> > &covers);
	void priorityInsert(const file_metadata &file);
	void thread();

	void transaction();
	void commit();

	int addParentDirectory(const std::string &dir, bool watch);
	bool isParentDirectory(int dir_id);
	QSqlQuery setParentDirectoryWatched(int dir_id, bool watched);
	QSqlQuery deleteDirectory(int dir_id);
	QSqlQuery deleteDirectory(const std::string &dir);
	QSqlQuery moveDirectory(const std::string &from, const std::string &to);
	QSqlQuery getParentDirectories();
	QSqlQuery getAllDirectories();

	QSqlQuery filterAudio(const std::string &needle, int limit = -1, int offset = 0);
	QSqlQuery filterByAlbum(const std::string &album, int limit = -1, int offset = 0);
	QSqlQuery filterByArtistAlbum(const std::string &artist, const std::string &album, int limit = -1, int offset = 0);
	QSqlQuery filterByAlbumArtistAlbum(const std::string &artist, const std::string &album, int limit = -1, int offset = 0);
	QSqlQuery filterByArtist(const std::string &artist, int limit = -1, int offset = 0);
	QSqlQuery filterByAlbumArtist(const std::string &artist, int limit = -1, int offset = 0);
	QSqlQuery filterByGenre(const std::string &genre, int limit = -1, int offset = 0);
	QSqlQuery filterByTitle(const std::string &title, int limit = -1, int offset = 0);

	QSqlQuery getAllArtists(int limit = -1, int offset = 0);
	QSqlQuery getAllAlbumArtists(int limit, int offset);
	QSqlQuery getArtists(const std::string &artist, int limit = -1, int offset = 0);
	QSqlQuery getAllAlbums(int limit = -1, int offset = 0);
	QSqlQuery getAlbums(const std::string &album, int limit = -1, int offset = 0);
	QSqlQuery getAlbumsByArtist(const std::string &artist, int limit = -1, int offset = 0);
	QSqlQuery getAlbumsByAlbumArtist(const std::string &artist, int limit = -1, int offset = 0);
	QSqlQuery getTracksByAlbumId(int id);

	QSqlQuery getAllAudio(int limit = -1, int offset = 0);
	QSqlQuery getAllVideos(int limit = -1, int offset = 0);
	QSqlQuery getVideos(const std::string &dir, const std::list<std::string> &tags = std::list<std::string>(), int limit = -1, int offset = 0);

	//Recordings
	QSqlQuery addRecording(const std::string &filepath, const std::string & title, int created);
	QSqlQuery getAllRecordings(int limit = -1, int offset = 0);
	QSqlQuery getRecordings(const std::string &dir, const std::list<std::string> &tags = std::list<std::string>(), int limit = -1, int offset = 0);
	QSqlQuery getRecordMeta(int file_id);
	QSqlQuery setRecordMeta(int file_id, const std::string & ref, const std::string &name, const std::string &description, const std::string &service_data, int64_t duration, int64_t filesize, int lastmodified, int created);
	QSqlQuery getRecordEit(int file_id);
	QSqlQuery setRecordEit(int file_id, const uint8_t eit_raw[], bool update=false);
	QSqlQuery getRecordTags(int file_id);
	QSqlQuery setRecordTags(int file_id, const std::vector<std::string> &tags);
	QSqlQuery setLastPlayed(int file_id, int64_t lastplayed, int64_t lastplaypos);
	QSqlQuery getLastPlayed(int file_id);

	QSqlQuery getByFile(const std::string &filepath);
	QSqlQuery getAudioByFile(const std::string &filepath);
	QSqlQuery getVideoByFile(const std::string &filepath);

	QSqlQuery getById(int id);
	QSqlQuery getAudioById(int id);
	QSqlQuery getVideoById(int id);

	QSqlQuery getByParentDirectoryId(int id, int limit=-1, int offset=0);

	QSqlQuery setFileAttribute(int file_id, const std::string &key, const std::string &value);
	QSqlQuery deleteFileAttribute(int file_id, const std::string &key);
	QSqlQuery getFileAttributes(int file_id);
	QSqlQuery getFileAttribute(int file_id, const std::string &key);
	QSqlQuery getFileAttributeValue(int file_id, const std::string &key);

	QSqlQuery getCoverArt(const std::string &key);
	QSqlQuery getCoverArt(int cover_art_id);
	QSqlQuery getAlbumCoverArt(int album_id);
	QSqlQuery getAlbumCoverArtId(int album_id);

	QSqlQuery addPlaylist(const std::string &name, int type);
	QSqlQuery getPlaylistById(int id);
	QSqlQuery getPlaylistByName(const std::string &name, int type);
	QSqlQuery getPlaylistItemsById(int id);
	QSqlQuery getPlaylistItemsByName(const std::string &name, int type);
	QSqlQuery getPlaylists(int type);
	QSqlQuery addToPlaylistById(int playlist_id, int file_id, int pos);
	QSqlQuery addToPlaylistByUri(int playlist_id, const std::string &uri, int pos, const stringMap &attributes=stringMap());
	void condensePlaylistItemAttributes(stringMap *item);
	QSqlQuery updatePlaylistItemInternal(int playlist_id, int item_id, int file_id, const std::string &file_uri, int pos, stringMap *attributes);
	QSqlQuery updatePlaylistItem(int playlist_id, stringMap *attributes);
	QSqlQuery getPlaylistItemAttribute(int playlist_item_id, const std::string &key);
	QSqlQuery getPlaylistItemAttributes(int playlist_item_id);
	QSqlQuery clearPlaylist(int id);
	QSqlQuery savePlaylist(int playlist_id, const std::string &name, int type, const stringMapVector &items);

	QSqlQuery removeFile(int file_id);

	QSqlQuery query(const std::string &sql, const stringList &values=stringList(), bool rw = false);

	void addRescanPath(const std::string &path);

	Signal4<void, uint64_t, uint64_t, uint64_t, std::list<int> > insertFinished;
	Signal5<void, char*, uint64_t, uint64_t, uint64_t, int> priorityInsertFinished;

private:
	enum {
		COMMIT_ERROR=-3,
		INSERT_ERROR=-2,
		INSERT_SKIPPED=-1,
	};

	struct DbMessage
	{
		int type;
		char* filename;
		uint64_t errors;
		uint64_t successful;
		uint64_t skipped;
		int file_id;
		enum
		{
			quit=-1,
			update,
			done_list,
			done_single
		};

		DbMessage(int type=0, char* filename=0, uint64_t errors=0, uint64_t successful=0, uint64_t skipped=0, int file_id=0) :
		type(type), filename(filename), errors(errors), successful(successful), skipped(skipped), file_id(file_id) {}
	};

	eFixedMessagePump<DbMessage> m_messages_to_thread;
	eFixedMessagePump<DbMessage> m_messages_from_thread;

	bool m_immediate_cancel;

	std::list<file_metadata> m_priority_files;
	std::map<std::string, std::list<file_metadata> > m_dirFiles;
	std::list<file_metadata> m_files;
	std::list<int> m_inserted_ids;
	std::list<std::string> m_directories;
	std::list<std::string> m_rescanPaths;
	std::map<uint32_t, ePtr<cover_art> > m_covers;
	std::string m_parent_dir;
	int m_parent_dir_id;

	QSqlDatabase m_db_rw, m_db_ro;

	QSqlQuery m_qry_insert_file;
	QSqlQuery m_qry_insert_directory;
	QSqlQuery m_qry_insert_codec;
	QSqlQuery m_qry_insert_audio_track;
	QSqlQuery m_qry_insert_audio_meta;
	QSqlQuery m_qry_insert_artist;
	QSqlQuery m_qry_insert_album;
	QSqlQuery m_qry_insert_genre;
	QSqlQuery m_qry_insert_cover;
	QSqlQuery m_qry_insert_video;
	QSqlQuery m_qry_insert_location;
	QSqlQuery m_qry_insert_tag;

	QSqlQuery m_qry_delete_file;
	QSqlQuery m_qry_update_file;
	QSqlQuery m_qry_update_audio_meta;
	QSqlQuery m_qry_update_album;

	QSqlQuery m_qry_get_file_by_id;
	QSqlQuery m_qry_get_file_lastmodified;
	QSqlQuery m_qry_get_directory_by_id;
	QSqlQuery m_qry_get_directory_by_path;
	QSqlQuery m_qry_get_codec_by_id;
	QSqlQuery m_qry_get_codec_by_name;
	QSqlQuery m_qry_get_audio_tracks_by_file_id;
	QSqlQuery m_qry_get_audio_meta_by_file_id;
	QSqlQuery m_qry_get_artist_by_id;
	QSqlQuery m_qry_get_artist_by_name;
	QSqlQuery m_qry_get_album_by_id;
	QSqlQuery m_qry_get_album_by_name;
	QSqlQuery m_qry_get_genre_by_id;
	QSqlQuery m_qry_get_genre_by_name;
	QSqlQuery m_qry_get_video_by_file_id;
	QSqlQuery m_qry_get_location_by_id;
	QSqlQuery m_qry_get_tag_by_id;
	QSqlQuery m_qry_get_tag_by_name;

	QSqlQuery m_qry_check_file;
	QSqlQuery m_qry_check_directory;
	QSqlQuery m_qry_check_codec;
	QSqlQuery m_qry_check_audio_track;
	QSqlQuery m_qry_check_audio_meta;
	QSqlQuery m_qry_check_artist;
	QSqlQuery m_qry_check_album;
	QSqlQuery m_qry_check_genre;
	QSqlQuery m_qry_check_cover;
	QSqlQuery m_qry_check_tag;

	void checkDeletedFiles(int parent_id, const std::list<file_metadata> files);
	bool haveFiles();
	void processFileLists();
	insert_result processSingleFile(const file_metadata &fmd, bool has_parent=false);
	void gotMessage(const DbMessage &message);

	QString genSelectAllById(const QString &table);
	QString genSelectAllBySingleField(const QString &table, const QString &field);
	QString genSelectGetIdSimple(const QString &table, const QString &field);

	const long long getIdSimple(QSqlQuery *qry, bool clear = true);
	insert_result insertError(int id=INSERT_ERROR);
	insert_result insertAndCleanup(QSqlQuery *qry);
	bool updateAndCleanup(QSqlQuery *qry);

	insert_result getSetDirectory(int parent_id, const std::string &path, bool parent = false, bool watch = false);
	insert_result getSetFile(int dir_id, const std::string &name, int64_t size, int type, int64_t duration, int64_t lastmodified, int popularity=0, int64_t lastplaypos=0, int cover_art_id=0);
	int getFileLastModified(int dir_id, const std::string &name, int64_t *timestamp);
	insert_result getSetTag(const std::string &tag);
	insert_result getSetCodec(const std::string &codec, const std::string &codec_long);
	insert_result setAudioTrack(int file_id, int codec_id, const std::string &lang, bool deleteOld=false);
	insert_result getSetArtist(const std::string &artist);
	insert_result getSetAlbum(int artist_id, int album_artist_id, const std::string &album);
	insert_result getSetGenre(const std::string &genre);
	insert_result getSetCover(uint32_t key, ePtr<cover_art> cover);
	insert_result setAudioMeta(int file_id, int artist_id, int album_artist_id, int album_id, int genre_id, const audio_metadata &amd);
	insert_result setVideoTrack(int file_id, int codec_id, const video_data &vd);
};
#endif //eDatabaseThread
#endif
