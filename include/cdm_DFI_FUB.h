#ifndef _CDM_DFI_FUB_H_
#define _CDM_DFI_FUB_H_

/*
###################################################################################
#
# CDMlib - Cartesian Data Management library
#
# Copyright (c) 2013-2017 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
 */

#include "cdm_DFI.h"

/**
 * @file   cdm_DFI_FUB.h
 * @brief  cdm_DFI_FUB Class Header
 * @author aics
 */

class cdm_DFI_FUB : public cdm_DFI {

public:

  cdm_FieldFileNameFormat DFI_FieldFileNameFormat; ///< FieldFileNameFormat class

protected:

/*
  struct fubFname
  {
    int    id; ///<read file rank ID
    string cFname; ///< Coordinate data fub file name
    string fFname; ///< Field data fub file name
  };

  vector<fubFname> fubFlist;
*/

public:

  /** コンストラクタ */
  cdm_DFI_FUB();

  /** コンストラクタ
   * @brief コンストラクタ
   * @param [in] F_Info  FileInfo
   * @param [in] F_Path  FilePath
   * @param [in] visit   VisIt option
   * @param [in] unit    Unit
   * @param [in] domain  Domain
   * @param [in] mpi     MPI
   * @param [in] process Process
   */
  cdm_DFI_FUB(const cdm_FileInfo F_Info,
              const cdm_FilePath F_Path,
              const cdm_VisIt visit,
              const cdm_Unit unit,
              const cdm_Domain *domain,
              const cdm_MPI mpi,
              const cdm_TimeSlice TSlice,
              const cdm_Process process)
  {
    DFI_Finfo     = F_Info;
    DFI_Fpath     = F_Path;
    DFI_VisIt     = visit;
    DFI_Unit      = unit;
    DFI_Domain    = domain;
    DFI_MPI       = mpi;
    DFI_TimeSlice = TSlice;
    DFI_Process   = process;
    m_bgrid_interp_flag = false;
    m_input_type  = CDM::E_CDM_FILE_TYPE_FBINARY;
    m_output_type = CDM::E_CDM_FILE_TYPE_FBINARY;
  };

  /** デストラクタ */
  ~cdm_DFI_FUB();

protected:
  /**
   * @brief sphファイルのヘッダーレコード読込み
   * @param[in]  pFile       ファイルポインタ
   * @param[in]  matchEndian エンディアンチェックフラグ true:合致
   * @param[in]  step        ステップ番号
   * @param[in]  head        dfiのHeadIndex
   * @param[in]  tail        dfiのTailIndex
   * @param[in]  gc          dfiのガイドセル数
   * @param[out] voxsize     voxsize
   * @param[out] time        時刻
   * @return error code
   */
  CDM::E_CDM_ERRORCODE
  read_HeaderRecord(cdm_FILE* pFile,
                    bool matchEndian,
                    unsigned step,
                    const int head[3],
                    const int tail[3],
                    int gc,
                    int voxsize[3],
                    double &time);

  /**
   * @brief フィールドデータファイルのデータレコード読込み
   * @param[in]  pFile       ファイルポインタ
   * @param[in]  matchEndian true:Endian一致
   * @param[in]  step        読込みstep番号
   * @param[in]  buf         読込み用バッファ
   * @param[in]  head        読込みバッファHeadIndex
   * @param[in]  nz          z方向のボクセルサイズ（実セル＋ガイドセル＊２）
   * @param[out] src         読み込んだデータを格納した配列のポインタ
   * @return error code
   */
  CDM::E_CDM_ERRORCODE
  read_Datarecord(cdm_FILE* pFile,
                  bool matchEndian,
                  unsigned step,
                  cdm_Array* buf,
                  int head[3],
                  int nz,
                  cdm_Array* &src);

  /**
  * @brief Averageデータレコードの読込み
  * @param[in]  pFile       ファイルポインタ
  * @param[in]  matchEndian true:Endian一致
  * @param[in]  step        読込みstep番号
  * @param[out] avr_step    平均ステップ
  * @param[out] avr_time    平均タイム
  * @return error code
  */
  CDM::E_CDM_ERRORCODE
  read_averaged(cdm_FILE* pFile,
                bool matchEndian,
                unsigned step,
                unsigned &avr_step,
                double &avr_time);

  /**
   * @brief ヘッダファイルの出力
   * @param[in] pFile  ファイルポインタ
   * @param[in] step   ステップ番号
   * @param[in] time   時刻
   * @param[in] RankID ランク番号
   * @return error code
   */
  CDM::E_CDM_ERRORCODE
  write_HeaderRecord(cdm_FILE* pFile,
                     const unsigned step,
                     const double time,
                     const int RankID);

  /**
   * @brief データレコードの出力
   * @param[in]  pFile ファイルポインタ
   * @param[in]  val データポインタ
   * @param[in]  gc ガイドセル
   * @param[in]  RankID ランク番号
   * @return error code
   */
  CDM::E_CDM_ERRORCODE
  write_DataRecord(cdm_FILE* pFile,
                   cdm_Array* val,
                   const int gc,
                   const int RankID);

  /**
   * @brief Averageレコードの出力
   *
   */
  CDM::E_CDM_ERRORCODE
  write_averaged(cdm_FILE* pFile,
                 const unsigned step_avr,
                 const double time_avr);

public:

  /**
   * @brief fub特有のFileList要素を読み込む
   * @param [in] tpCntl cdm_TextParserクラス
   * @return error code
   */

/*
  CDM::E_CDM_ERRORCODE
  readFileListTP( cdm_TextParser tpCntl );
*/
  /**
   * @brief field data file nameの取得
   * @details index dfiファイルの/FileList/RankのIDと読み込み指示されたランクのID
   * と一致したIDのRankで定義されたFieldDataFileNameで指示されたファイル名もしくは
   * CoordinateFileNameで指示されたファイル名を取得（E_CDM_FMT_FUBのときは
   * FieldDataFileName、E_CDM_FMT_FUB_CODのときはCoordinateFileName)
   * index dfiファイルにFileListがないファイルは空白が戻る.
   *
   * @param [in] ID RankID
   * @return field data file name
   */
/*
  std::string
  getFileNameFromFileList( const int ID );
*/
  /**
   * @brief coordinate file nameの取得
   * @details index dfi のFileListに定義してある、フィールドファイルとペアになって
   * いる座標値ファイル名をフィールドファイル名で検索して取得、index dfiにFileList
   * がないときはフィールドファイル名の拡張子をxyzにしたファイル名が戻る．
   *
   * @param [in] FieldFileName field data file name
   * @return coordinate data file name
   */
  std::string
  getCoordinateFileName(std::string FieldFileName);

  /**
  * @brief read xyz(fub) data record
  * @details template ReadData関数で型に応じた配列を確保した後、呼び出される
  * @param [out] val        読み込み先の配列をポインタで渡す　
  * @param [in]  step       読み込むステップ番号
  * @param [in]  gc         仮想セル数　
  * @param [in]  Gvoxel     グローバルボクセルサイズ
  * @param [in]  Gdivision  領域分割数　
  * @param [in]  head       計算領域の開始位置　　
  * @param [in]  tail       計算領域の終了位置
  * @param [out] time       読み込んだ時間
  * @param [in]  mode       平均ステップ＆時間読込みフラグ　false : 読込み
  *                                                         true  : 読み込まない
  * @param [out] step_avr      平均ステップ
  * @param [out] time_avr      平均時間
  * @return 終了コード 1:正常 1以外:エラー
  */
  CDM::E_CDM_ERRORCODE
  ReadCoordinateData(cdm_Array *val,
           const unsigned step,
           const int gc,
           const int Gvoxel[3],
           const int Gdivision[3],
           const int head[3],
           const int tail[3],
           double &time,
           const bool mode,
           unsigned &step_avr,
           double &time_avr);
  /**
   * @brief write coordinate data record
   * @param [in] step     出力ステップ番号
   * @param [in] gc       仮想セル数
   * @param [in] time     出力時刻
   * @param [in] val      出力データポインタ
   */
  CDM::E_CDM_ERRORCODE
  WriteCoordinateData(const unsigned step,
                      const int gc,
                      double time,
                      cdm_Array* val);

  /**
   * @brief index.dfiにFileListがあるとき、FileListを出力
   * @param [in] fp  ファイルポインタ
   * @param [in] tab インデント
   * @return error code
   */
/*
  CDM::E_CDM_ERRORCODE
  WriteFileList(FILE *fp,
                const unsigned tab);
*/
  /**
   * @brief cdm_FieldFileNameFormatクラスのポインタ取得
   * @return cdm_FieldFileNameFormatクラスのポインタ
   */
  const cdm_FieldFileNameFormat* GetcdmFieldFileNameFormat();

  /**
   *
   *
   */
  void SetcdmFieldFileNameFormat(cdm_FieldFileNameFormat _FieldFileNameFormat)
  { DFI_FieldFileNameFormat = _FieldFileNameFormat; };

};

#endif
