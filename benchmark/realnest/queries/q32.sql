SELECT 
  list_aggregate(tau_pt, 'string_agg', '|'),
  list_aggregate(tau_eta, 'string_agg', '|'),
  list_aggregate(jet_pt, 'string_agg', '|'),
  list_aggregate(jet_eta, 'string_agg', '|'),
  list_aggregate(muon_pt, 'string_agg', '|'),
  list_aggregate(muon_eta, 'string_agg', '|'),
  list_aggregate(el_pt, 'string_agg', '|'),
  list_aggregate(el_eta, 'string_agg', '|'),
  list_aggregate(ph_pt, 'string_agg', '|'),
  list_aggregate(ph_eta, 'string_agg', '|')  
FROM singleMu;