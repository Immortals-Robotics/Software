#pragma once

class ConfigReader
{
public:
    explicit ConfigReader(std::string_view t_file_path);

    void load();

    [[nodiscard]] auto getRoot() const
    {
        return static_cast<toml::node_view<const toml::node>>(m_table);
    }

private:
    std::filesystem::path m_file_path;

    toml::table m_table;
};

struct IConfig
{
protected:
    ~IConfig() = default;

public:
    virtual void load(toml::node_view<const toml::node> t_node) = 0;
};
